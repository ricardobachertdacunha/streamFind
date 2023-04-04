// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

#ifdef RCPP_USE_GLOBAL_ROSTREAM
Rcpp::Rostream<true>&  Rcpp::Rcout = Rcpp::Rcpp_cout_get();
Rcpp::Rostream<false>& Rcpp::Rcerr = Rcpp::Rcpp_cerr_get();
#endif

// rcpp_ms_cluster_ms2
List rcpp_ms_cluster_ms2(DataFrame ms2, double mzClust, bool verbose);
RcppExport SEXP _streamFind_rcpp_ms_cluster_ms2(SEXP ms2SEXP, SEXP mzClustSEXP, SEXP verboseSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< DataFrame >::type ms2(ms2SEXP);
    Rcpp::traits::input_parameter< double >::type mzClust(mzClustSEXP);
    Rcpp::traits::input_parameter< bool >::type verbose(verboseSEXP);
    rcpp_result_gen = Rcpp::wrap(rcpp_ms_cluster_ms2(ms2, mzClust, verbose));
    return rcpp_result_gen;
END_RCPP
}
// rcpp_ms_cluster_spectra
List rcpp_ms_cluster_spectra(DataFrame spectra, double mzClust, bool verbose);
RcppExport SEXP _streamFind_rcpp_ms_cluster_spectra(SEXP spectraSEXP, SEXP mzClustSEXP, SEXP verboseSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< DataFrame >::type spectra(spectraSEXP);
    Rcpp::traits::input_parameter< double >::type mzClust(mzClustSEXP);
    Rcpp::traits::input_parameter< bool >::type verbose(verboseSEXP);
    rcpp_result_gen = Rcpp::wrap(rcpp_ms_cluster_spectra(spectra, mzClust, verbose));
    return rcpp_result_gen;
END_RCPP
}
// rcpp_ms_feature_groups_correspondence
bool rcpp_ms_feature_groups_correspondence(Rcpp::DataFrame groups, Rcpp::DataFrame features, bool verbose);
RcppExport SEXP _streamFind_rcpp_ms_feature_groups_correspondence(SEXP groupsSEXP, SEXP featuresSEXP, SEXP verboseSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::DataFrame >::type groups(groupsSEXP);
    Rcpp::traits::input_parameter< Rcpp::DataFrame >::type features(featuresSEXP);
    Rcpp::traits::input_parameter< bool >::type verbose(verboseSEXP);
    rcpp_result_gen = Rcpp::wrap(rcpp_ms_feature_groups_correspondence(groups, features, verbose));
    return rcpp_result_gen;
END_RCPP
}
// rcpp_ms_get_feature_groups_ranges
DataFrame rcpp_ms_get_feature_groups_ranges(StringVector groups, Rcpp::DataFrame features);
RcppExport SEXP _streamFind_rcpp_ms_get_feature_groups_ranges(SEXP groupsSEXP, SEXP featuresSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< StringVector >::type groups(groupsSEXP);
    Rcpp::traits::input_parameter< Rcpp::DataFrame >::type features(featuresSEXP);
    rcpp_result_gen = Rcpp::wrap(rcpp_ms_get_feature_groups_ranges(groups, features));
    return rcpp_result_gen;
END_RCPP
}
// rcpp_ms_make_groups_update_features
List rcpp_ms_make_groups_update_features(Rcpp::DataFrame features);
RcppExport SEXP _streamFind_rcpp_ms_make_groups_update_features(SEXP featuresSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::DataFrame >::type features(featuresSEXP);
    rcpp_result_gen = Rcpp::wrap(rcpp_ms_make_groups_update_features(features));
    return rcpp_result_gen;
END_RCPP
}
// rcpp_ms_update_groups
DataFrame rcpp_ms_update_groups(Rcpp::DataFrame features, Rcpp::StringVector analyses);
RcppExport SEXP _streamFind_rcpp_ms_update_groups(SEXP featuresSEXP, SEXP analysesSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::DataFrame >::type features(featuresSEXP);
    Rcpp::traits::input_parameter< Rcpp::StringVector >::type analyses(analysesSEXP);
    rcpp_result_gen = Rcpp::wrap(rcpp_ms_update_groups(features, analyses));
    return rcpp_result_gen;
END_RCPP
}
// parse_msAnalysis_from_mzml
Rcpp::List parse_msAnalysis_from_mzml(std::string file_path);
RcppExport SEXP _streamFind_parse_msAnalysis_from_mzml(SEXP file_pathSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type file_path(file_pathSEXP);
    rcpp_result_gen = Rcpp::wrap(parse_msAnalysis_from_mzml(file_path));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_streamFind_rcpp_ms_cluster_ms2", (DL_FUNC) &_streamFind_rcpp_ms_cluster_ms2, 3},
    {"_streamFind_rcpp_ms_cluster_spectra", (DL_FUNC) &_streamFind_rcpp_ms_cluster_spectra, 3},
    {"_streamFind_rcpp_ms_feature_groups_correspondence", (DL_FUNC) &_streamFind_rcpp_ms_feature_groups_correspondence, 3},
    {"_streamFind_rcpp_ms_get_feature_groups_ranges", (DL_FUNC) &_streamFind_rcpp_ms_get_feature_groups_ranges, 2},
    {"_streamFind_rcpp_ms_make_groups_update_features", (DL_FUNC) &_streamFind_rcpp_ms_make_groups_update_features, 1},
    {"_streamFind_rcpp_ms_update_groups", (DL_FUNC) &_streamFind_rcpp_ms_update_groups, 2},
    {"_streamFind_parse_msAnalysis_from_mzml", (DL_FUNC) &_streamFind_parse_msAnalysis_from_mzml, 1},
    {NULL, NULL, 0}
};

RcppExport void R_init_streamFind(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
