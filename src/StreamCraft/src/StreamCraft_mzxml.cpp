#include "StreamCraft_mzxml.hpp"
#include <omp.h>
#include <filesystem>
#include <cstring>
#include <algorithm>
#include <set>

int sc::mzxml::MZXML_SPECTRUM::extract_spec_index() const {
  return spec.attribute("num").as_int();
};

std::string sc::mzxml::MZXML_SPECTRUM::extract_spec_id() const {
  return spec.attribute("num").as_string();
};

int sc::mzxml::MZXML_SPECTRUM::extract_spec_scan() const {
  return spec.attribute("num").as_int();
};

int sc::mzxml::MZXML_SPECTRUM::extract_spec_array_length() const {
  return spec.attribute("peaksCount").as_int();
};

int sc::mzxml::MZXML_SPECTRUM::extract_spec_level() const {
  return spec.attribute("msLevel").as_int();
};

std::string sc::mzxml::MZXML_SPECTRUM::extract_spec_mode() const {
  int centroided = spec.attribute("centroided").as_int();
  if (centroided == 1) {
    return "centroid";
  } else if (centroided == 0) {
     return "profile";
  } else {
    return "";
  }
};

std::string sc::mzxml::MZXML_SPECTRUM::extract_spec_polarity() const {
  std::string pol_sign = spec.attribute("polarity").as_string();
  if (pol_sign == "+") {
     return "positive";
  } else if (pol_sign == "-") {
    return "negative";
  } else {
    return "";
  }
};

double sc::mzxml::MZXML_SPECTRUM::extract_spec_lowmz() const {
  return spec.attribute("lowMz").as_double();
};

double sc::mzxml::MZXML_SPECTRUM::extract_spec_highmz() const {
  return spec.attribute("highMz").as_double();
};

double sc::mzxml::MZXML_SPECTRUM::extract_spec_bpmz() const {
  return spec.attribute("basePeakMz").as_double();
};

double sc::mzxml::MZXML_SPECTRUM::extract_spec_bpint() const {
  return spec.attribute("basePeakIntensity").as_double();
};

double sc::mzxml::MZXML_SPECTRUM::extract_spec_tic() const {
  return spec.attribute("totIonCurrent").as_double();
};

double sc::mzxml::MZXML_SPECTRUM::extract_scan_rt() const {
  std::string rt = spec.attribute("retentionTime").as_string();
  double rt_n;
  std::sscanf(rt.c_str(), "%*[^0123456789]%lf", &rt_n);
  char last_char = '\0';
  std::sscanf(rt.c_str() + rt.size() - 1, "%c", &last_char);
  if (last_char != 'S') rt_n = rt_n * 60;
  return rt_n;
};

double sc::mzxml::MZXML_SPECTRUM::extract_ion_mz() const {
  pugi::xml_node precursor = spec.child("precursorMz");
  return precursor.text().as_double();
};

double sc::mzxml::MZXML_SPECTRUM::extract_activation_ce() const {
  return spec.attribute("collisionEnergy").as_double();
};

sc::MZXML_BINARY_METADATA sc::mzxml::MZXML_SPECTRUM::extract_binary_metadata() const {
  
  sc::MZXML_BINARY_METADATA binary_metadata;
  
  binary_metadata.precision = spec.child("peaks").attribute("precision").as_int();

  std::string compression = spec.child("peaks").attribute("compressionType").as_string();

  binary_metadata.compression = compression;

  if (compression == "zlib" || compression == "zlib compression") {
    binary_metadata.compressed = true;

  } else {
    binary_metadata.compressed = false;
  }

  std::string byte_order = spec.child("peaks").attribute("byteOrder").as_string();

  if (byte_order == "network") {
    binary_metadata.byte_order = "big_endian";

  } else {
    binary_metadata.byte_order = "little_endian";
  }

  return binary_metadata;
};

std::vector<std::vector<double>> sc::mzxml::MZXML_SPECTRUM::extract_binary_data(const MZXML_BINARY_METADATA& mtd) const {

  std::vector<std::vector<double>> spectrum(2);

  const int number_traces = spec.attribute("peaksCount").as_int();

  if (number_traces == 0) return  spectrum;

  for (int i = 0; i < 2; i++) spectrum[i].resize(number_traces);

  const char* encoded_string = spec.child("peaks").child_value();

  std::string decoded_string = utils::decode_base64(encoded_string);

  if (mtd.compressed) {
    decoded_string = utils::decompress_zlib(decoded_string);
  }

  std::vector<double> res(number_traces * 2);

  if (mtd.byte_order == "big_endian") {
    res = utils::decode_big_endian(decoded_string, mtd.precision / 8);

  } else if (mtd.byte_order == "little_endian") {
    res = utils::decode_little_endian(decoded_string, mtd.precision / 8);

  } else {
    std::cerr << "Byte order must be big_endian or little_endian!" << std::endl;
    return(spectrum);
  }

  for (int i = 0; i < number_traces; i++) {
    spectrum[0][i] = res[i * 2];
    spectrum[1][i] = res[i * 2 + 1];
  }

  return spectrum;
};

std::vector<pugi::xml_node> sc::mzxml::MZXML::link_vector_spectra_nodes() const {

  std::vector<pugi::xml_node> spectra;

  pugi::xml_node msrun = root.child("msRun");

  if (msrun) {
    for (pugi::xml_node child = msrun.child("scan"); child; child = child.next_sibling()) {
      spectra.push_back(child);
    }

  } else {
    std::cerr << "msRun not found in the mzXML file!" << std::endl;
  }
  
  return spectra;
};

sc::mzxml::MZXML::MZXML(const std::string& file) {

  file_path = file;

  file_dir = file.substr(0, file.find_last_of("/\\") + 1);

  if (file_dir.back() == '/') file_dir.pop_back();

  file_name = file.substr(file.find_last_of("/\\") + 1);
  
  file_extension = file_name.substr(file_name.find_last_of(".") + 1);
  
  file_name = file_name.substr(0, file_name.find_last_of("."));

  const char* path = file.c_str();

  loading_result = doc.load_file(path, pugi::parse_default | pugi::parse_declaration | pugi::parse_pi);

  if (loading_result) {
    root = doc.document_element();

    if (!root) {
      std::cerr << "Root element is empty!" << std::endl;

    } else {
      format = root.name();

      if ("mzXML" == format) {
        format = "mzXML";

      } else {
        std::cerr << "Root element must be mzXML!" << std::endl;
      }
    }

  } else {
    std::cerr << "mzXML file could not be opened!" << std::endl << loading_result.description() << std::endl;
  }

  name = root.name();

  if (get_number_spectra() > 0) spectra_nodes = link_vector_spectra_nodes();
};

void sc::mzxml::MZXML::print() const {
  std::cout << name << std::endl;
  std::cout << std::endl;
  std::cout << " File:                      " << file_path << std::endl;
  std::cout << std::endl;
  std::cout << " Number of spectra:         " << get_number_spectra() << std::endl;
  // std::cout << " Spectra mode (first):      " << first_spectra_headers.spec_mode[0] << std::endl;
  std::cout << std::endl;
};

int sc::mzxml::MZXML::get_number_spectra() const {
  const pugi::xml_node msrun = root.child("msRun");
  return msrun.attribute("scanCount").as_int();
};

int sc::mzxml::MZXML::get_number_spectra_binary_arrays() const {
  if (get_number_spectra() == 0) return 0;
  else return 1;
};

std::vector<std::string> sc::mzxml::MZXML::get_spectra_binary_short_names() const {
  if (get_number_spectra() == 0) return std::vector<std::string> {};
  else return std::vector<std::string> {"mz", "intensity"};
};

sc::MZXML_BINARY_METADATA sc::mzxml::MZXML::get_spectra_binary_metadata() const {
  const pugi::xml_node msrun = root.child("msRun");
  const pugi::xml_node spec = msrun.child("scan");
  return MZXML_SPECTRUM(spec).extract_binary_metadata();
};

std::string sc::mzxml::MZXML::get_type() const {
  const int number_spectra = get_number_spectra();
  std::string type = "Unknown";
  if (number_spectra > 0) {
    const std::vector<int>& level = get_level();
    const std::vector<double>& pre_mz = get_spectra_precursor_mz();
    bool no_pre_mz = std::all_of(pre_mz.begin(), pre_mz.end(), [](double d) { return std::isnan(d); });
    if (level.size() > 1) {
      if (no_pre_mz) {
        type = "MS/MS-AllIons";
      } else {
        type = "MS/MS-DDA";
      }
    } else if (level[0] == 1) {
      type = "MS";
    } else {
      type = "MSn";
    }
  }
  return type;
};

std::vector<int> sc::mzxml::MZXML::get_spectra_index(std::vector<int> indices) const {

  const int number_spectra = get_number_spectra();
  
  std::vector<int> idxs;

  if (number_spectra == 0) {
    std::cerr << "There are no spectra in the mzXML file!" << std::endl;
    return idxs;
  }

  if (indices.size() == 0) {
    indices.resize(number_spectra);
    std::iota(indices.begin(), indices.end(), 0);
  }

  const int n = indices.size();
  const std::vector<int> f_indices = indices;

  idxs.resize(n);

  // #pragma omp parallel for
  for (int i = 0; i < n; ++i) {
    const int& idx = f_indices[i];
    const sc::MZXML_SPECTRUM& spec(spectra_nodes[idx]);
    idxs[i] = spec.extract_spec_index();
  }

  return idxs;
};

std::vector<int> sc::mzxml::MZXML::get_spectra_scan_number(std::vector<int> indices) const {

  const int number_spectra = get_number_spectra();
  
  std::vector<int> scans;

  if (number_spectra == 0) {
    std::cerr << "There are no spectra in the mzXML file!" << std::endl;
    return scans;
  }

  if (indices.size() == 0) {
    indices.resize(number_spectra);
    std::iota(indices.begin(), indices.end(), 0);
  }

  const int n = indices.size();
  const std::vector<int> f_indices = indices;

  scans.resize(n);

  // #pragma omp parallel for
  for (int i = 0; i < n; ++i) {
    const int& idx = f_indices[i];
    const sc::MZXML_SPECTRUM& spec(spectra_nodes[idx]);
    scans[i] = spec.extract_spec_scan();
  }

  return scans;
};

std::vector<int> sc::mzxml::MZXML::get_spectra_array_length(std::vector<int> indices) const {

  const int number_spectra = get_number_spectra();
  
  std::vector<int> lengths;

  if (number_spectra == 0) {
    std::cerr << "There are no spectra in the mzXML file!" << std::endl;
    return lengths;
  }

  if (indices.size() == 0) {
    indices.resize(number_spectra);
    std::iota(indices.begin(), indices.end(), 0);
  }

  const int n = indices.size();
  const std::vector<int> f_indices = indices;

  lengths.resize(n);

  // #pragma omp parallel for
  for (int i = 0; i < n; ++i) {
    const int& idx = f_indices[i];
    const sc::MZXML_SPECTRUM& spec(spectra_nodes[idx]);
    lengths[i] = spec.extract_spec_array_length();
  }

  return lengths;
};

std::vector<int> sc::mzxml::MZXML::get_spectra_level(std::vector<int> indices) const {

  const int number_spectra = get_number_spectra();
  
  std::vector<int> levels;

  if (number_spectra == 0) {
    std::cerr << "There are no spectra in the mzXML file!" << std::endl;
    return levels;
  }

  if (indices.size() == 0) {
    indices.resize(number_spectra);
    std::iota(indices.begin(), indices.end(), 0);
  }

  const int n = indices.size();
  const std::vector<int> f_indices = indices;

  levels.resize(n);

  // #pragma omp parallel for
  for (int i = 0; i < n; ++i) {
    const int& idx = f_indices[i];
    const sc::MZXML_SPECTRUM& spec(spectra_nodes[idx]);
    levels[i] = spec.extract_spec_level();
  }

  return levels;
};

std::vector<std::string> sc::mzxml::MZXML::get_spectra_mode(std::vector<int> indices) const {

  const int number_spectra = get_number_spectra();
  
  std::vector<std::string> modes;

  if (number_spectra == 0) {
    std::cerr << "There are no spectra in the mzXML file!" << std::endl;
    return modes;
  }

  if (indices.size() == 0) {
    indices.resize(number_spectra);
    std::iota(indices.begin(), indices.end(), 0);
  }

  const int n = indices.size();
  const std::vector<int> f_indices = indices;

  modes.resize(n);

  // #pragma omp parallel for
  for (int i = 0; i < n; ++i) {
    const int& idx = f_indices[i];
    const sc::MZXML_SPECTRUM& spec(spectra_nodes[idx]);
    modes[i] = spec.extract_spec_mode();
  }

  return modes;
};

std::vector<std::string> sc::mzxml::MZXML::get_spectra_polarity(std::vector<int> indices) const {

  const int number_spectra = get_number_spectra();
  
  std::vector<std::string> polarities;

  if (number_spectra == 0) {
    std::cerr << "There are no spectra in the mzXML file!" << std::endl;
    return polarities;
  }

  if (indices.size() == 0) {
    indices.resize(number_spectra);
    std::iota(indices.begin(), indices.end(), 0);
  }

  const int n = indices.size();
  const std::vector<int> f_indices = indices;

  polarities.resize(n);

  // #pragma omp parallel for
  for (int i = 0; i < n; ++i) {
    const int& idx = f_indices[i];
    const sc::MZXML_SPECTRUM& spec(spectra_nodes[idx]);
    polarities[i] = spec.extract_spec_polarity();
  }

  return polarities;
};

std::vector<double> sc::mzxml::MZXML::get_spectra_lowmz(std::vector<int> indices) const {

  const int number_spectra = get_number_spectra();
  
  std::vector<double> lowmzs;

  if (number_spectra == 0) {
    std::cerr << "There are no spectra in the mzXML file!" << std::endl;
    return lowmzs;
  }

  if (indices.size() == 0) {
    indices.resize(number_spectra);
    std::iota(indices.begin(), indices.end(), 0);
  }

  const int n = indices.size();
  const std::vector<int> f_indices = indices;

  lowmzs.resize(n);

  // #pragma omp parallel for
  for (int i = 0; i < n; ++i) {
    const int& idx = f_indices[i];
    const sc::MZXML_SPECTRUM& spec(spectra_nodes[idx]);
    lowmzs[i] = spec.extract_spec_lowmz();
  }

  return lowmzs;
};

std::vector<double> sc::mzxml::MZXML::get_spectra_highmz(std::vector<int> indices) const {

  const int number_spectra = get_number_spectra();
  
  std::vector<double> highmzs;

  if (number_spectra == 0) {
    std::cerr << "There are no spectra in the mzXML file!" << std::endl;
    return highmzs;
  }

  if (indices.size() == 0) {
    indices.resize(number_spectra);
    std::iota(indices.begin(), indices.end(), 0);
  }

  const int n = indices.size();
  const std::vector<int> f_indices = indices;

  highmzs.resize(n);

  // #pragma omp parallel for
  for (int i = 0; i < n; ++i) {
    const int& idx = f_indices[i];
    const sc::MZXML_SPECTRUM& spec(spectra_nodes[idx]);
    highmzs[i] = spec.extract_spec_highmz();
  }

  return highmzs;
};

std::vector<double> sc::mzxml::MZXML::get_spectra_bpmz(std::vector<int> indices) const {

  const int number_spectra = get_number_spectra();
  
  std::vector<double> bpmzs;

  if (number_spectra == 0) {
    std::cerr << "There are no spectra in the mzXML file!" << std::endl;
    return bpmzs;
  }

  if (indices.size() == 0) {
    indices.resize(number_spectra);
    std::iota(indices.begin(), indices.end(), 0);
  }

  const int n = indices.size();
  const std::vector<int> f_indices = indices;

  bpmzs.resize(n);

  // #pragma omp parallel for
  for (int i = 0; i < n; ++i) {
    const int& idx = f_indices[i];
    const sc::MZXML_SPECTRUM& spec(spectra_nodes[idx]);
    bpmzs[i] = spec.extract_spec_bpmz();
  }

  return bpmzs;
};

std::vector<double> sc::mzxml::MZXML::get_spectra_bpint(std::vector<int> indices) const {

  const int number_spectra = get_number_spectra();
  
  std::vector<double> bpints;

  if (number_spectra == 0) {
    std::cerr << "There are no spectra in the mzXML file!" << std::endl;
    return bpints;
  }

  if (indices.size() == 0) {
    indices.resize(number_spectra);
    std::iota(indices.begin(), indices.end(), 0);
  }

  const int n = indices.size();
  const std::vector<int> f_indices = indices;

  bpints.resize(n);

  // #pragma omp parallel for
  for (int i = 0; i < n; ++i) {
    const int& idx = f_indices[i];
    const sc::MZXML_SPECTRUM& spec(spectra_nodes[idx]);
    bpints[i] = spec.extract_spec_bpint();
  }

  return bpints;
};

std::vector<double> sc::mzxml::MZXML::get_spectra_tic(std::vector<int> indices) const {

  const int number_spectra = get_number_spectra();
  
  std::vector<double> tics;

  if (number_spectra == 0) {
    std::cerr << "There are no spectra in the mzXML file!" << std::endl;
    return tics;
  }

  if (indices.size() == 0) {
    indices.resize(number_spectra);
    std::iota(indices.begin(), indices.end(), 0);
  }

  const int n = indices.size();
  const std::vector<int> f_indices = indices;

  tics.resize(n);

  // #pragma omp parallel for
  for (int i = 0; i < n; ++i) {
    const int& idx = f_indices[i];
    const sc::MZXML_SPECTRUM& spec(spectra_nodes[idx]);
    tics[i] = spec.extract_spec_tic();
  }

  return tics;
};

std::vector<double> sc::mzxml::MZXML::get_spectra_rt(std::vector<int> indices) const {

  const int number_spectra = get_number_spectra();
  
  std::vector<double> rts;

  if (number_spectra == 0) {
    std::cerr << "There are no spectra in the mzXML file!" << std::endl;
    return rts;
  }

  if (indices.size() == 0) {
    indices.resize(number_spectra);
    std::iota(indices.begin(), indices.end(), 0);
  }

  const int n = indices.size();
  const std::vector<int> f_indices = indices;

  rts.resize(n);

  // #pragma omp parallel for
  for (int i = 0; i < n; ++i) {
    const int& idx = f_indices[i];
    const sc::MZXML_SPECTRUM& spec(spectra_nodes[idx]);
    rts[i] = spec.extract_scan_rt();
  }

  return rts;
};

std::vector<double> sc::mzxml::MZXML::get_spectra_precursor_mz(std::vector<int> indices) const {

  const int number_spectra = get_number_spectra();
  
  std::vector<double> mzs;

  if (number_spectra == 0) {
    std::cerr << "There are no spectra in the mzXML file!" << std::endl;
    return mzs;
  }

  if (indices.size() == 0) {
    indices.resize(number_spectra);
    std::iota(indices.begin(), indices.end(), 0);
  }

  const int n = indices.size();
  const std::vector<int> f_indices = indices;

  mzs.resize(n);

  // #pragma omp parallel for
  for (int i = 0; i < n; ++i) {
    const int& idx = f_indices[i];
    const sc::MZXML_SPECTRUM& spec(spectra_nodes[idx]);
    mzs[i] = spec.extract_ion_mz();
  }

  return mzs;
};

std::vector<double> sc::mzxml::MZXML::get_spectra_collision_energy(std::vector<int> indices) const {

  const int number_spectra = get_number_spectra();
  
  std::vector<double> energies;

  if (number_spectra == 0) {
    std::cerr << "There are no spectra in the mzXML file!" << std::endl;
    return energies;
  }

  if (indices.size() == 0) {
    indices.resize(number_spectra);
    std::iota(indices.begin(), indices.end(), 0);
  }

  const int n = indices.size();
  const std::vector<int> f_indices = indices;

  energies.resize(n);

  // #pragma omp parallel for
  for (int i = 0; i < n; ++i) {
    const int& idx = f_indices[i];
    const sc::MZXML_SPECTRUM& spec(spectra_nodes[idx]);
    energies[i] = spec.extract_activation_ce();
  }

  return energies;
};

std::vector<std::string> sc::mzxml::MZXML::get_polarity() const {
  const std::vector<std::string>& polarity = get_spectra_polarity();
  std::set<std::string> unique_polarity(polarity.begin(), polarity.end());
  return std::vector<std::string>(unique_polarity.begin(), unique_polarity.end());
};

std::vector<std::string> sc::mzxml::MZXML::get_mode() const {
  const std::vector<std::string>& mode = get_spectra_mode();
  std::set<std::string> unique_mode(mode.begin(), mode.end());
  return std::vector<std::string>(unique_mode.begin(), unique_mode.end());
};

std::vector<int> sc::mzxml::MZXML::get_level() const {
  const std::vector<int>& levels = get_spectra_level();
  std::set<int> unique_level(levels.begin(), levels.end());
  return std::vector<int>(unique_level.begin(), unique_level.end());
};

double sc::mzxml::MZXML::get_min_mz() const {
  const std::vector<double>& mz_low = get_spectra_lowmz();
  return *std::min_element(mz_low.begin(), mz_low.end());
};

double sc::mzxml::MZXML::get_max_mz() const {
  const std::vector<double>& mz_high = get_spectra_highmz();
  return *std::max_element(mz_high.begin(), mz_high.end());
};

double sc::mzxml::MZXML::get_start_rt() const {
  const std::vector<double>& rt = get_spectra_rt();
  return *std::min_element(rt.begin(), rt.end());
};

double sc::mzxml::MZXML::get_end_rt() const {
  const std::vector<double>& rt = get_spectra_rt();
  return *std::max_element(rt.begin(), rt.end());
};

sc::MS_SUMMARY sc::mzxml::MZXML::get_summary() const {
  sc::MS_SUMMARY summary;
  summary.file_name = file_name;
  summary.file_path = file_path;
  summary.file_dir = file_dir;
  summary.file_extension = file_extension;
  summary.number_spectra = get_number_spectra();
  summary.number_chromatograms = get_number_chromatograms();
  summary.number_spectra_binary_arrays = get_number_spectra_binary_arrays();
  summary.format = get_format();
  summary.type = get_type();
  summary.polarity = get_polarity();
  summary.mode = get_mode();
  summary.level = get_level();
  summary.min_mz = get_min_mz();
  summary.max_mz = get_max_mz();
  summary.start_rt = get_start_rt();
  summary.end_rt = get_end_rt();
  summary.has_ion_mobility = has_ion_mobility();
  summary.time_stamp = get_time_stamp();
  return summary;
};

sc::MS_SPECTRA_HEADERS sc::mzxml::MZXML::get_spectra_headers(std::vector<int> indices) const {

  const int number_spectra = get_number_spectra();

  sc::MS_SPECTRA_HEADERS headers;

  if (number_spectra == 0) {
    std::cerr << "There are no spectra in the mzXML file!" << std::endl;
    return headers;
  }

  if (indices.size() == 0) {
    indices.resize(number_spectra);
    std::iota(indices.begin(), indices.end(), 0);
  }

  const std::vector<int> idxs = indices;
  
  const int n = idxs.size();

  if (n == 0) {
    std::cerr << "No indices given!" << std::endl;
    return headers;
  }

  if (spectra_nodes.size() == 0) {
    std::cerr << "No spectra found!" << std::endl;
    return headers;
  }

  headers.resize_all(n);

  // // #pragma omp parallel for
  for (int i = 0; i < n; i++) {
    const int& index = idxs[i];
    const sc::MZXML_SPECTRUM& sp = spectra_nodes[index];

    headers.index[i] = sp.extract_spec_index();
    headers.scan[i] = sp.extract_spec_scan();
    headers.array_length[i] = sp.extract_spec_array_length();
    headers.level[i] = sp.extract_spec_level();
    headers.mode[i] = sp.extract_spec_mode();
    headers.polarity[i] = sp.extract_spec_polarity();
    headers.lowmz[i] = sp.extract_spec_lowmz();
    headers.highmz[i] = sp.extract_spec_highmz();
    headers.bpmz[i] = sp.extract_spec_bpmz();
    headers.bpint[i] = sp.extract_spec_bpint();
    headers.tic[i] = sp.extract_spec_tic();
    headers.rt[i] = sp.extract_scan_rt();

    if (sp.has_precursor()) {
      headers.precursor_mz[i] = sp.extract_ion_mz();
      headers.activation_ce[i] = sp.extract_activation_ce();
    }
  } // end of i loop

  return headers;
};

std::vector<std::vector<std::vector<double>>> sc::mzxml::MZXML::get_spectra(std::vector<int> indices) const {

  const int number_spectra = get_number_spectra();

  std::vector<std::vector<std::vector<double>>> sp;

  if (number_spectra == 0) {
    std::cerr << "There are no spectra in the mzXML file!" << std::endl;
    return sp;
  }

  if (indices.size() == 0) {
    indices.resize(number_spectra);
    std::iota(indices.begin(), indices.end(), 0);
  }

  const std::vector<int> idxs = indices;

  const int n = idxs.size();

  if (n == 0) {
    std::cerr << "No indices given!" << std::endl;
    return sp;
  }

  if (spectra_nodes.size() == 0) {
    std::cerr << "No spectra found!" << std::endl;
    return sp;
  }

  sp.resize(n);

  const sc::MZXML_BINARY_METADATA binary_metadata = get_spectra_binary_metadata();

  #pragma omp parallel for
  for (int i = 0; i < n; i++) {
    const int& index = idxs[i];
    const sc::MZXML_SPECTRUM& spec = spectra_nodes[index];
    sp[i] = spec.extract_binary_data(binary_metadata);
  }

  return sp;
};

std::vector<std::vector<std::string>> sc::mzxml::MZXML::get_software() const {

  std::vector<std::vector<std::string>> output(3);

  std::string search_software = "//msInstrument/child::node()[starts-with(name(), 'soft')]";
  pugi::xpath_node_set xps_software = root.select_nodes(search_software.c_str());

  if (xps_software.size() > 0) {
    for (pugi::xpath_node_set::const_iterator it = xps_software.begin(); it != xps_software.end(); ++it) {
      pugi::xpath_node node = *it;
      output[0].push_back(node.node().attribute("name").as_string());
      output[1].push_back(node.node().attribute("type").as_string());
      output[2].push_back(node.node().attribute("version").as_string());
    }
  }

  return output;
};

std::vector<std::vector<std::string>> sc::mzxml::MZXML::get_hardware() const {

  std::vector<std::vector<std::string>> output(2);

  std::string search_inst = "//msInstrument/child::node()[starts-with(name(), 'ms')]";
  pugi::xpath_node_set xps_inst = root.select_nodes(search_inst.c_str());

  if (xps_inst.size() > 0) {
    for (pugi::xpath_node_set::const_iterator it = xps_inst.begin(); it != xps_inst.end(); ++it) {
      pugi::xpath_node node = *it;
      output[0].push_back(node.node().attribute("category").as_string());
      output[1].push_back(node.node().attribute("value").as_string());
    }
  }

  return output;
};

void sc::mzxml::test_extract_spectra_mzxml(const std::string& file) {
  
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << "Test Extract Spectra mzXML file:" << std::endl;
  std::cout << std::endl;

  MZXML mzxml(file);

  std::cout << "Root name: " << mzxml.name << std::endl;

  std::cout << "Number of spectra: " << mzxml.get_number_spectra() << std::endl;

  MS_SPECTRA_HEADERS hd;

  hd = mzxml.get_spectra_headers();

  int number = hd.index.size();

  std::cout << "Size of vector in headers struct: " << number << std::endl;

  std::cout << "Retention time of 10th spectrum: " << hd.rt[10] << std::endl;

  std::cout << "Number of binary arrays: " << mzxml.get_number_spectra_binary_arrays() << std::endl;

  std::vector<std::vector<std::vector<double>>> spectra;

  std::vector<int> indices = {10, 15};

  spectra = mzxml.get_spectra(indices);

  std::cout << "Number of extracted spectra: " << spectra.size() << std::endl;

  std::cout << "Number of traces in the first extracted spectrum: " << spectra[0][0].size() << std::endl;

  std::cout << std::endl;
  
};
