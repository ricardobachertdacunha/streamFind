#include <vector>
#include <string>
#include <Rcpp.h>

#define STREAMCRAFT_HEADER_ONLY
#include "StreamCraft/src/StreamCraft_lib.hpp"

// [[Rcpp::export]]
Rcpp::List rcpp_parse_ms_analysis_v2(std::string file_path) {
  
  Rcpp::List list_out;
  
  Rcpp::CharacterVector na_charvec(1, NA_STRING);
  
  Rcpp::DataFrame empty_df;
  
  empty_df.attr("class") = Rcpp::CharacterVector::create("data.table", "data.frame");
  
  Rcpp::List empty_list;
  
  sc::MS_ANALYSIS ana(file_path);
  
  list_out["name"] = ana.file_name;
  
  list_out["replicate"] = na_charvec;
  
  list_out["blank"] = na_charvec;
  
  list_out["file"] = ana.file_path;
  
  list_out["format"] = ana.get_format();
  
  list_out["type"] = ana.get_type();
  
  list_out["spectra_number"] = ana.get_number_spectra();
  
  if (ana.get_number_spectra() > 0) {
    sc::MS_SPECTRA_HEADERS hd = ana.get_spectra_headers();
    
    Rcpp::List hdl;
    
    hdl["index"] = hd.index;
    // hdl["id"] = hd.id;
    hdl["scan"] = hd.scan;
    hdl["array_length"] = hd.array_length;
    hdl["level"] = hd.level;
    hdl["mode"] = hd.mode;
    hdl["polarity"] = hd.polarity;
    hdl["lowmz"] = hd.lowmz;
    hdl["highmz"] = hd.highmz;
    hdl["bpmz"] = hd.bpmz;
    hdl["bpint"] = hd.bpint;
    hdl["tic"] = hd.tic;
    // hdl["title"] = hd.title;
    hdl["rt"] = hd.rt;
    hdl["drift"] = hd.drift;
    // hdl["filter_string"] = hd.filter_string;
    // hdl["config"] = hd.config;
    // hdl["injection_ion_time"] = hd.injection_ion_time;
    hdl["pre_scan"] = hd.precursor_scan;
    hdl["window_mz"] = hd.window_mz;
    hdl["pre_mzlow"] = hd.window_mzlow;
    hdl["pre_mzhigh"] = hd.window_mzhigh;
    hdl["pre_mz"] = hd.precursor_mz;
    hdl["pre_charge"] = hd.precursor_charge;
    hdl["pre_intensity"] = hd.precursor_intensity;
    // hdl["pre_type"] = hd.activation_type;
    hdl["pre_ce"] = hd.activation_ce;
    
    hdl.attr("class") = Rcpp::CharacterVector::create("data.table", "data.frame");
    
    list_out["spectra_headers"] = hdl;
    
  } else {
    list_out["spectra_headers"] = empty_df;
  }
  
  list_out["spectra"] = empty_df;
  
  list_out["chromatograms_number"] = ana.get_number_chromatograms();
  
  if (ana.get_number_chromatograms() > 0) {
    
    sc::MS_CHROMATOGRAMS_HEADERS hd2 = ana.get_chromatograms_headers();
    
    Rcpp::List hdl2;
    
    hdl2["index"] = hd2.index;
    hdl2["id"] = hd2.id;
    hdl2["array_length"] = hd2.array_length;
    hdl2["polarity"] = hd2.polarity;
    hdl2["pre_mz"] = hd2.precursor_mz;
    hdl2["pro_mz"] = hd2.product_mz;
    // hdl2["activation_type"] = hd2.activation_type;
    hdl2["pre_ce"] = hd2.activation_ce;
    
    hdl2.attr("class") = Rcpp::CharacterVector::create("data.table", "data.frame");
    
    list_out["chromatograms_headers"] = hdl2;
    
  } else {
    list_out["chromatograms_headers"] = empty_df;
  }
  
  list_out["chromatograms"] = empty_df;
  
  list_out["metadata"] = empty_list;
  
  list_out.attr("class") = Rcpp::CharacterVector::create("MassSpecAnalysis", "Analysis");
  
  return list_out;
}

// [[Rcpp::export]]
Rcpp::List rcpp_parse_ms_spectra_headers_v2(std::string file_path) {
  
  Rcpp::List list_out;
  
  sc::MS_ANALYSIS ana(file_path);
  
  if (ana.get_number_spectra() == 0) return list_out;
  
  sc::MS_SPECTRA_HEADERS headers = ana.get_spectra_headers();
  
  list_out["index"] = headers.index;
  // list_out["id"] = headers.id;
  list_out["scan"] = headers.scan;
  list_out["array_length"] = headers.array_length;
  list_out["level"] = headers.level;
  list_out["mode"] = headers.mode;
  list_out["polarity"] = headers.polarity;
  list_out["lowmz"] = headers.lowmz;
  list_out["highmz"] = headers.highmz;
  list_out["bpmz"] = headers.bpmz;
  list_out["bpint"] = headers.bpint;
  list_out["tic"] = headers.tic;
  // list_out["title"] = headers.title;
  list_out["rt"] = headers.rt;
  list_out["drift"] = headers.drift;
  // list_out["filter_string"] = headers.filter_string;
  // list_out["config"] = headers.config;
  // list_out["injection_ion_time"] = headers.injection_ion_time;
  list_out["pre_scan"] = headers.precursor_scan;
  list_out["window_mz"] = headers.window_mz;
  list_out["pre_mzlow"] = headers.window_mzlow;
  list_out["pre_mzhigh"] = headers.window_mzhigh;
  list_out["pre_scan"] = headers.precursor_scan;
  list_out["pre_mz"] = headers.precursor_mz;
  list_out["pre_charge"] = headers.precursor_charge;
  list_out["pre_intensity"] = headers.precursor_intensity;
  // list_out["activation_type"] = headers.activation_type;
  list_out["pre_ce"] = headers.activation_ce;
  
  list_out.attr("class") = Rcpp::CharacterVector::create("data.table", "data.frame");
  
  return list_out;
}

// [[Rcpp::export]]
Rcpp::List rcpp_parse_ms_chromatograms_headers_v2(std::string file_path) {
  
  Rcpp::List list_out;
  
  sc::MS_ANALYSIS ana(file_path);
  
  if (ana.get_number_chromatograms() == 0) return list_out;
  
  sc::MS_CHROMATOGRAMS_HEADERS headers = ana.get_chromatograms_headers();
  
  list_out["index"] = headers.index;
  list_out["id"] = headers.id;
  list_out["array_length"] = headers.array_length;
  list_out["polarity"] = headers.polarity;
  list_out["pre_mz"] = headers.precursor_mz;
  list_out["pro_mz"] = headers.product_mz;
  // list_out["activation_type"] = headers.activation_type;
  list_out["pre_ce"] = headers.activation_ce;
  
  list_out.attr("class") = Rcpp::CharacterVector::create("data.table", "data.frame");
  
  return list_out;
}

// [[Rcpp::export]]
Rcpp::List rcpp_parse_ms_spectra_v2(Rcpp::List analysis,
                                    std::vector<int> levels,
                                    Rcpp::DataFrame targets,
                                    double minIntensityMS1,
                                    double minIntensityMS2) {
  
  Rcpp::DataFrame empty_df;
  empty_df.attr("class") = Rcpp::CharacterVector::create("data.table", "data.frame");
  
  Rcpp::List out;
  
  const std::string file = analysis["file"];
  
  const Rcpp::List& hd = analysis["spectra_headers"];
  
  const std::vector<int> polarity = hd["polarity"];
  const std::vector<int> level = hd["level"];
  const std::vector<double> pre_mz = hd["pre_mz"];
  // const std::vector<double> pre_mzlow = hd["pre_mzlow"];
  // const std::vector<double> pre_mzhigh = hd["pre_mzhigh"];
  const std::vector<double> pre_ce = hd["pre_ce"];
  const std::vector<double> rt = hd["rt"];
  const std::vector<double> drift = hd["drift"];
  
  const double minIntLv1 = minIntensityMS1;
  const double minIntLv2 = minIntensityMS2;

  const int number_spectra = rt.size();
  
  const int number_levels = levels.size();
  
  if (number_spectra == 0) return empty_df;
  
  std::vector<bool> level_filter(number_spectra, false);
  
  for (int i = 0; i < number_spectra; i++) {
    for (int j = 0; j < number_levels; j++) {
      if (level[i] == levels[j]) {
        level_filter[i] = true;
        break;
      }
    }
  }
  
  const int n_tg = targets.nrow();
  
  sc::MS_ANALYSIS ana(file);

  if (n_tg == 0) {
    
    std::vector<int> indices;
    
    for (int i = 0; i < number_spectra; i++) if (level_filter[i]) indices.push_back(i);
    
    const std::vector<std::vector<std::vector<double>>> spectra = ana.get_spectra(indices);
    
    const int number_spectra_extracted = spectra.size();
    
    if (number_spectra != number_spectra_extracted) return empty_df;
    
    const int number_arrays = spectra[0].size();
    
    if (number_arrays == 0) return empty_df;
    
    int total_traces = 0;
    
    for (int i = 0; i < number_spectra; i++) total_traces += spectra[i][0].size();
    
    std::vector<int> polarity_out(total_traces);
    std::vector<int> level_out(total_traces);
    std::vector<double> pre_mz_out(total_traces);
    // std::vector<double> pre_mzlow_out(total_traces);
    // std::vector<double> pre_mzhigh_out(total_traces);
    std::vector<double> pre_ce_out(total_traces);
    std::vector<double> rt_out(total_traces);
    std::vector<double> drift_out(total_traces);
    std::vector<double> mz_out(total_traces);
    std::vector<double> intensity_out(total_traces);

    int trace = 0;
    
    for (int i = 0; i < number_spectra; i++) {
      const std::vector<double>& mz_ref = spectra[i][0];
      const std::vector<double>& intensity_ref = spectra[i][1];
      const int n = mz_ref.size();
      
      for (int k = 0; k < n; k++) {
        polarity_out[trace] = polarity[i];
        level_out[trace] = level[i];
        pre_mz_out[trace] = pre_mz[i];
        // pre_mzlow_out[trace] = pre_mzlow[i];
        // pre_mzhigh_out[trace] = pre_mzhigh[i];
        pre_ce_out[trace] = pre_ce[i];
        rt_out[trace] = rt[i];
        drift_out[trace] = drift[i];
        mz_out[trace] = mz_ref[k];
        intensity_out[trace] = intensity_ref[k];

        trace += 1;
      }
    }
    
    out["polarity"] = polarity_out;
    out["level"] = level_out;
    out["pre_mz"] = pre_mz_out;
    // out["pre_mzlow"] = pre_mzlow_out;
    // out["pre_mzhigh"] = pre_mzhigh_out;
    out["pre_ce"] = pre_ce_out;
    out["rt"] = rt_out;
    out["drift"] = drift_out;
    out["mz"] = mz_out;
    out["intensity"] = intensity_out;
    
    out.attr("class") = Rcpp::CharacterVector::create("data.table", "data.frame");
    return out;
    
  } else {
    
    std::vector<int> tg_idx(n_tg);
    std::iota(tg_idx.begin(), tg_idx.end(), 0);
    
    const std::vector<std::string> df_id = targets["id"];
    std::vector<int> dummy_level(n_tg);
    const std::vector<int> df_polarity = targets["polarity"];
    const std::vector<bool> df_precursor = targets["precursor"];
    const std::vector<double> df_mzmin = targets["mzmin"];
    const std::vector<double> df_mzmax = targets["mzmax"];
    const std::vector<double> df_rtmin = targets["rtmin"];
    const std::vector<double> df_rtmax = targets["rtmax"];
    const std::vector<double> df_driftmin = targets["driftmin"];
    const std::vector<double> df_driftmax = targets["driftmax"];
    
    sc::MS_TARGETS tg = {
      tg_idx,
      df_id,
      dummy_level,
      df_polarity,
      df_precursor,
      df_mzmin,
      df_mzmax,
      df_rtmin,
      df_rtmax,
      df_driftmin,
      df_driftmax
    };

    std::set<int> idx;

    for (int i = 0; i < n_tg; i++) {
      for (int j = 0; j < number_spectra; j++) {

        if (level_filter[j]) {
          if ((rt[j] >= tg.rtmin[i] && rt[j] <= tg.rtmax[i]) || tg.rtmax[i] == 0) {
            if (polarity[j] == tg.polarity[i]) {
              if ((drift[j] >= tg.driftmin[i] && drift[j] <= tg.driftmax[i]) || tg.driftmax[i] == 0) {
                if (tg.precursor[i]) {
                  if ((pre_mz[j] >= tg.mzmin[i] && pre_mz[j] <= tg.mzmax[i]) || tg.mzmax[i] == 0) {
                    idx.insert(j);
                  }
                } else {
                  idx.insert(j);
                }
              }
            }
          }
        }
      }
    }

    std::vector<int> idx_vector(idx.begin(), idx.end());

    std::sort(idx_vector.begin(), idx_vector.end());

    if (idx_vector.size() == 0) return empty_df;

    const std::vector<std::vector<std::vector<double>>> spectra = ana.get_spectra(idx_vector);

    const int number_spectra_targets = spectra.size();

    if (number_spectra_targets == 0) return empty_df;

    std::vector<std::vector<std::vector<double>>> res(n_tg);

    std::vector<std::vector<int>> res_idx(n_tg);

    // #pragma omp parallel for
    for (int i = 0; i < n_tg; i++) {

      res[i].resize(2);

      for (int j = 0; j < number_spectra_targets; j++) {

        const int& tg_polarity = polarity[idx_vector[j]];
        const int& tg_level = level[idx_vector[j]];
        const double& tg_pre_mz = pre_mz[idx_vector[j]];
        // const double& tg_pre_mzlow = pre_mzlow[idx_vector[j]];
        // const double& tg_pre_mzhigh = pre_mzhigh[idx_vector[j]];
        const double& tg_rt = rt[idx_vector[j]];
        const double& tg_drift = drift[idx_vector[j]];

        if (tg.polarity[i] == tg_polarity) {

          if (tg.rtmax[i] == 0 || (tg_rt >= tg.rtmin[i] && tg_rt <= tg.rtmax[i])) {

            if (tg.driftmax[i] == 0 || (tg_drift >= tg.driftmin[i] && tg_drift <= tg.driftmax[i])) {

              if (tg.precursor[i]) {

                if ((tg_pre_mz >= tg.mzmin[i] && tg_pre_mz <= tg.mzmax[i]) || tg.mzmax[i] == 0) {

                  const std::vector<double>& mzj = spectra[j][0];
                  const std::vector<double>& intj = spectra[j][1];

                  const int number_points = mzj.size();

                  if (number_points == 0) continue;

                  for (int k = 0; k < number_points; k++) {

                    if (intj[k] >= minIntLv2 && tg_level == 2) {
                      res[i][0].push_back(mzj[k]);
                      res[i][1].push_back(intj[k]);
                      res_idx[i].push_back(idx_vector[j]);
                    }
                  }
                }
              } else {

                const std::vector<double>& mzj = spectra[j][0];
                const std::vector<double>& intj = spectra[j][1];

                const int number_points = mzj.size();

                if (number_points == 0) continue;

                for (int k = 0; k < number_points; k++) {

                  if ((mzj[k] >= tg.mzmin[i] && mzj[k] <= tg.mzmax[i]) || tg.mzmax[i] == 0) {

                    if ((intj[k] >= minIntLv2 && tg_level == 2) || (intj[k] >= minIntLv1 && tg_level == 1)) {
                      res[i][0].push_back(mzj[k]);
                      res[i][1].push_back(intj[k]);
                      res_idx[i].push_back(idx_vector[j]);
                    }
                  }
                }
              }
            }
          }
        }
      }
    }

    int total_traces = 0;

    for (int i = 0; i < n_tg; i++) total_traces += res[i][0].size();

    if (total_traces == 0) return empty_df;

    std::vector<std::string> id_out(total_traces);
    std::vector<int> polarity_out(total_traces);
    std::vector<int> level_out(total_traces);
    std::vector<double> pre_mz_out(total_traces);
    // std::vector<double> pre_mzlow_out(total_traces);
    // std::vector<double> pre_mzhigh_out(total_traces);
    std::vector<double> pre_ce_out(total_traces);
    std::vector<double> rt_out(total_traces);
    std::vector<double> drift_out(total_traces);
    std::vector<double> mz_out(total_traces);
    std::vector<double> intensity_out(total_traces);

    int trace = 0;

    for (int i = 0; i < n_tg; i++) {

      const int n = res[i][0].size();

      if (n == 0) continue;

      for (int j = 0; j < n; j++) {

        id_out[trace] = tg.id[i];
        polarity_out[trace] = polarity[res_idx[i][j]];
        level_out[trace] = level[res_idx[i][j]];
        pre_mz_out[trace] = pre_mz[res_idx[i][j]];
        // pre_mzlow_out[trace] = pre_mzlow[res_idx[i][j]];
        // pre_mzhigh_out[trace] = pre_mzhigh[res_idx[i][j]];
        pre_ce_out[trace] = pre_ce[res_idx[i][j]];
        rt_out[trace] = rt[res_idx[i][j]];
        drift_out[trace] = drift[res_idx[i][j]];
        mz_out[trace] = res[i][0][j];
        intensity_out[trace] = res[i][1][j];

        trace += 1;
      }
    }

    out["id"] = id_out;
    out["polarity"] = polarity_out;
    out["level"] = level_out;
    out["pre_mz"] = pre_mz_out;
    // out["pre_mzlow"] = pre_mzlow_out;
    // out["pre_mzhigh"] = pre_mzhigh_out;
    out["pre_ce"] = pre_ce_out;
    out["rt"] = rt_out;
    out["drift"] = drift_out;
    out["mz"] = mz_out;
    out["intensity"] = intensity_out;

    out.attr("class") = Rcpp::CharacterVector::create("data.table", "data.frame");

    return out;
  }
  
  return empty_df;
}

// [[Rcpp::export]]
Rcpp::List rcpp_parse_ms_chromatograms_v2(Rcpp::List analysis, std::vector<int> idx) {
  
  Rcpp::DataFrame empty_df;
  empty_df.attr("class") = Rcpp::CharacterVector::create("data.table", "data.frame");
  
  Rcpp::List out;
  
  const std::string file = analysis["file"];
  
  const Rcpp::List& hd = analysis["chromatograms_headers"];
  
  const std::vector<int> index = hd["index"];
  const std::vector<std::string> id = hd["id"];
  const std::vector<int> polarity = hd["polarity"];
  const std::vector<double> pre_mz = hd["pre_mz"];
  const std::vector<double> pre_ce = hd["pre_ce"];
  const std::vector<double> pro_mz = hd["pro_mz"];
  
  const int number_chromatograms = index.size();
  
  if (number_chromatograms == 0) return empty_df;
  
  sc::MS_ANALYSIS ana(file);
  
  if (idx.size() == 0) idx = index;
  
  const std::vector<std::vector<std::vector<double>>> chromatograms = ana.get_chromatograms(idx);
  
  const int number_extracted_chromatograms = chromatograms.size();
  
  if (number_extracted_chromatograms == 0) return empty_df;
  
  int total_traces = 0;
  
  for (int i = 0; i < number_extracted_chromatograms; i++) total_traces += chromatograms[i][0].size();
  
  if (total_traces == 0) return empty_df;
    
  std::vector<int> index_out(total_traces);
  std::vector<std::string> id_out(total_traces);
  std::vector<int> polarity_out(total_traces);
  std::vector<double> pre_mz_out(total_traces);
  std::vector<double> pre_ce_out(total_traces);
  std::vector<double> pro_mz_out(total_traces);
  std::vector<double> rt_out(total_traces);
  std::vector<double> intensity_out(total_traces);
    
  int trace = 0;
    
  for (int i = 0; i < number_extracted_chromatograms; i++) {
    
    const std::vector<double>& rtj = chromatograms[i][0];
    const std::vector<double>& intj = chromatograms[i][1];
 
    const int n = rtj.size();
    
    if (n == 0) continue;
      
    for (int j = 0; j < n; j++) {
      
      index_out[trace] = index[i];
      id_out[trace] = id[i];
      polarity_out[trace] = polarity[i];
      pre_mz_out[trace] = pre_mz[i];
      pre_ce_out[trace] = pre_ce[i];
      pro_mz_out[trace] = pro_mz[i];
      rt_out[trace] = rtj[j];
      intensity_out[trace] = intj[j];
      
      trace += 1;
    }
  }
    
  out["index"] = index_out;
  out["id"] = id_out;
  out["polarity"] = polarity_out;
  out["pre_mz"] = pre_mz_out;
  out["pre_ce"] = pre_ce_out;
  out["pro_mz"] = pro_mz_out;
  out["rt"] = rt_out;
  out["intensity"] = intensity_out;
  
  out.attr("class") = Rcpp::CharacterVector::create("data.table", "data.frame");
  
  return out;
}
