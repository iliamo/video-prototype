/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2014 Regents of the University of California.
 *
 * @author Lijing Wang <phdloli@ucla.edu>
 */

#include "video-generator.hpp" 
// Enclosing code in ndn simplifies coding (can also use `using namespace ndn`)
namespace ndn {
// Additional nested namespace could be used to prevent/limit name contentions

  time_t time_start;
  VideoGenerator::Producer_Need pro_video;
  VideoGenerator::Producer_Need pro_audio;

  //when control-c detected, doing the analysis 
  static void sig_int(int num)
  {
    time_t time_end = std::time(0);
    double seconds = difftime(time_end, time_start);
    std::cout << "  " << std::endl;
    std::cout << "｡:.ﾟヽ(*´∀`)ﾉﾟ. Test Result ヽ(✿ﾟ▽ﾟ)ノ  " << std::endl;
    std::cout << "  " << std::endl;
    std::cout << "-------- Throughput --------  " << std::endl;
    std::cout << "Video Bytes: " << std::dec << pro_video.throughput << "  Payload_Throughput: " << std::dec << pro_video.throughput/seconds << " Bytes/Seconds" <<std::endl;
    std::cout << "Audio Bytes: " << std::dec << pro_audio.throughput << "  Payload_Throughput: " << std::dec << pro_audio.throughput/seconds << " Bytes/Seconds" <<std::endl;
    std::cout << "Total Bytes: " << std::dec << pro_video.throughput + pro_audio.throughput << "  Payload_Throughput: " << std::dec << (pro_video.throughput + pro_audio.throughput)/seconds << " Bytes/Seconds" << std::endl;

    unsigned long interest_in = pro_video.streaminfoCB.interest_incoming + pro_video.sampleCB.interest_incoming + pro_audio.streaminfoCB.interest_incoming + pro_audio.sampleCB.interest_incoming;
    unsigned long interest_out = pro_video.streaminfoCB.interest_outgoing + pro_video.sampleCB.interest_outgoing + pro_audio.streaminfoCB.interest_outgoing + pro_audio.sampleCB.interest_outgoing;
    unsigned long interest_noh = pro_video.streaminfoCB.interest_nohit + pro_video.sampleCB.interest_nohit + pro_audio.streaminfoCB.interest_nohit + pro_audio.sampleCB.interest_nohit;

    std::cout << "  " << std::endl;
    std::cout << "-------- Interest --------  " << std::endl;
    std::cout << "Interest_Incoming: " << std::dec << interest_in << "  Interest_Incoming_Speed: " << std::dec << interest_in/seconds << "/Seconds" << std::endl; 
    std::cout << "Interest_Outgoing: " << std::dec << interest_out << "  Interest_Outgoing_Speed: " << std::dec << interest_out/seconds << "/Seconds" << std::endl; 
    std::cout << "Interest_Nohit: " << std::dec << interest_noh << "  Interest_Nohit_Speed: " << std::dec << interest_noh/seconds << "/Seconds" << std::endl; 
    std::cout << "  " << std::endl;
    std::cout << "---------- OVER ----------  " << seconds <<" seconds" << std::endl;
   
    return;
  }

  int
  main(int argc, char** argv)
  {
    try {
      
      VideoGenerator generator;
      std::string filename = "/ndn/edu/ucla/capture";
      
      signal(SIGINT, sig_int);
      time_start = std::time(0);
      //call videogenerator
      generator.h264_generate_capture(filename, &pro_video, &pro_audio);

      std::cout << "ε٩(๑> ₃ <)۶з" << std::endl;

    }
    catch(std::exception& e) {
      std::cout << "HAHA" << std::endl;
      std::cerr << "ERROR: " << e.what() << std::endl;
      return 1;
    }
    return 0;
  }

} // namespace ndn

int
main(int argc, char** argv)
{
  return ndn::main(argc, argv);
}
