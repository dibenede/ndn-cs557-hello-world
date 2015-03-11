#include <iostream>

#include <ndn-cxx/face.hpp>
#include <ndn-cxx/security/key-chain.hpp>

class Producer
{
public:
  Producer(const std::string& uri)
    : m_prefix(uri)
  {
  }

  virtual void
  start()
  {
    m_face.setInterestFilter(ndn::InterestFilter(m_prefix),
                             bind(&Producer::onInterest, this, _1, _2),
                             bind(&Producer::onRegisterSuccess, this, _1),
                             bind(&Producer::onRegisterFailure, this, _1, _2));

    m_face.processEvents();
  }

  virtual void
  onRegisterSuccess(const ndn::Name& prefix)
  {
    std::cout << "Successfully registered " << prefix << std::endl;
  }

  virtual void
  onRegisterFailure(const ndn::Name& prefix, const std::string& reason)
  {
    std::cout << "Failed to register prefix " << prefix << ": " << reason << std::endl;
  }

protected:

  virtual void
  onInterest(const ndn::InterestFilter& filter, const ndn::Interest& interest)
  {
    std::cout << "Received Interest " << interest << std::endl;

    std::shared_ptr<ndn::Data> response(std::make_shared<ndn::Data>(interest.getName()));

    const std::string message = "Hello world!";
    response->setContent(reinterpret_cast<const uint8_t*>(message.c_str()),
                         message.size());

    m_keyChain.sign(*response);
    m_face.put(*response);
  }


protected:
  const ndn::Name m_prefix;
  ndn::Face m_face;
  ndn::KeyChain m_keyChain;
};


int
main(int argc, char** argv)
{
  Producer p("/hello/world");
  p.start();

  return 0;
}
