/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014 Regents of the University of California.
 *
 * @author Lijing Wang <phdloli@ucla.edu>
 */

#include "video-player.hpp"


namespace ndn {

  VideoPlayer::VideoPlayer()
  {
  }

  void
  VideoPlayer::get_streaminfo(std::string streaminfo)
  {
    VideoAudio *va =  &s_va;
    App *app = &(va->v_app);
    app->capstr = streaminfo; 
    std::cout << "Video streaminfo " << streaminfo << std::endl;
  }

  void
  VideoPlayer::get_streaminfo_audio(std::string streaminfo)
  {
    VideoAudio *va =  &s_va;
    App *app = &(va->a_app);
    app->capstr = streaminfo; 
    pthread_mutex_init(&(app->count_mutex), NULL);
    pthread_cond_init (&(app->count_cond), NULL);
    std::cout << "Audio streaminfo " << streaminfo << std::endl;
    h264_appsrc_init();
  }

  void
  VideoPlayer::h264_appsrc_data(const uint8_t* buffer, size_t bufferSize )
  {
    VideoAudio *va = &s_va;
    App *app = &(va->v_app);
    /* get some vitals, this will be used to read data from the mmapped file and
     * feed it to appsrc. */
    DataNode dataNode;
    uint8_t* bufferTmp = new uint8_t[bufferSize];
    memcpy (bufferTmp, buffer, bufferSize);
    dataNode.length = bufferSize;
    dataNode.data = (guint8 *) bufferTmp;
    (app->dataQue).push_back(dataNode);
    pthread_mutex_lock(&(app->count_mutex));
    if((app->dataQue).size() >= app->rate)
       pthread_cond_signal(&(app->count_cond));
    pthread_mutex_unlock(&(app->count_mutex));

    std::cout << "CP Video Done! " << bufferSize <<std::endl;
  }

  void
  VideoPlayer::h264_appsrc_data_audio(const uint8_t* buffer, size_t bufferSize )
  {
    VideoAudio *va = &s_va;
    App *app = &(va->a_app);
    DataNode dataNode;
    uint8_t* bufferTmp = new uint8_t[bufferSize];
    memcpy (bufferTmp, buffer, bufferSize);
    dataNode.length = bufferSize;
    dataNode.data = (guint8 *) bufferTmp;
    (app->dataQue).push_back(dataNode);
    pthread_mutex_lock(&(app->count_mutex));
    if((app->dataQue).size() >= app->rate)
       pthread_cond_signal(&(app->count_cond));
    pthread_mutex_unlock(&(app->count_mutex));

    std::cout << "CP Audio Done! " << bufferSize <<std::endl;
  }

/*
 * First INIT the h264_appsrc using thread
 * waiting in a loop
 */
  void
  VideoPlayer::h264_appsrc_init()
  {
    VideoAudio *va = &s_va;
    pthread_t thread; 
    int rc;
    rc = pthread_create(&thread, NULL, h264_appsrc_thread , (void *)va);
    std::cout << "h264_appsrc_init OK! " << std::endl;
  }
/* Call Consume Here From the start*/
  void
  VideoPlayer::consume_whole(Consumer *frameConsumer, Consumer *sampleConsumer)
  {
    VideoAudio *va = &s_va;
    App *video = &(va->v_app);
    App *audio = &(va->a_app);
    std::cout << "Inside the consume_whole! " <<std::endl;
    for(int seconds = 0; seconds < 60*10 ; seconds++)
    {
      for(int i=0; i< video->rate; i++)
      {
        Name frameSuffix("video/" + std::to_string(seconds*video->rate + i));
        frameConsumer->consume(frameSuffix);
      }
      for(int j=0; j< audio->rate; j++)
      {
        Name sampleSuffix("audio/" + std::to_string(seconds*audio->rate + j));
        sampleConsumer->consume(sampleSuffix);
      }
    }
  }
} // namespace ndn
