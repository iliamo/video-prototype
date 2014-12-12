/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2014 Regents of the University of California.
 *
 * @author Lijing Wang <phdloli@ucla.edu>
 */
#ifndef PRODUCER_CALLBACK_HPP
#define PRODUCER_CALLBACK_HPP
#include <ndn-cxx/contexts/producer-context.hpp>
#include <ndn-cxx/contexts/application-nack.hpp>
namespace ndn {
// Additional nested namespace could be used to prevent/limit name contentions
  class ProducerCallback
  {
  public:
    ProducerCallback();
    
    void
    setProducer(Producer* p);
    
    void
    setSampleNumber(size_t* n);

    void
    processConstData(const Data& data);
    
    void
    processInterest(const Interest& interest);
    
    void
    processOutgoingData(const Data& data)
    {
      std::cout << "OutgoingData " << data.getName() << std::endl;
//      std::cout << data.getFinalBlockId() << std::endl;
    }
    void
    processIncomingInterest(const Interest& interest);
    
    bool
    verifyInterest(Interest& interest);
    
    void
    processConstInterest(const Interest& interest);
  
  private:
    Producer* m_producer;
    size_t* m_curnum;
  };
  
} // namespace ndn
#endif
