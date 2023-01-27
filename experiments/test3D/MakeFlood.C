//
//
std::vector<std::string> paths;
void MakeFlood()
{
                // /media/7jn/T7/3DAM/3DAM/DAQ/run_8/RAW
  paths.push_back("/media/7jn/T7/3DAM/3DAM/DAQ/run_8/RAW/DataR_CH0@DT5730S_13949_run_8.root");
  paths.push_back("/media/7jn/T7/3DAM/3DAM/DAQ/run_8/RAW/DataR_CH1@DT5730S_13949_run_8.root");
  paths.push_back("/media/7jn/T7/3DAM/3DAM/DAQ/run_8/RAW/DataR_CH2@DT5730S_13949_run_8.root");
  paths.push_back("/media/7jn/T7/3DAM/3DAM/DAQ/run_8/RAW/DataR_CH3@DT5730S_13949_run_8.root");
  // ****************************
  LBDCalibration lc(paths,"junk");
  lc.createFlood("temp_flood.root");
}


