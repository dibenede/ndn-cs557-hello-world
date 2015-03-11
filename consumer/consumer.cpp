#include <iostream>

#include <ndn-cxx/face.hpp>

class Consumer
{
public:

  Consumer(const std::string& uri)
    : m_prefix(uri)
  {
  }

  virtual void
  start()
  {
    std::shared_ptr<ndn::Interest> request(std::make_shared<ndn::Interest>(m_prefix));
    m_face.expressInterest(*request,
                           bind(&Consumer::onData, this, _1, _2),
                           bind(&Consumer::onTimeout, this, _1));

    m_face.processEvents();
  }

protected:

  virtual void
  onData(const ndn::Interest& interest, const ndn::Data& data)
  {
    std::cout << "Received Data " << data << std::endl;

    const ndn::Block& payload = data.getContent();

    std:: cout << "payload: ";
    std::cout.write(reinterpret_cast<const char*>(payload.value()), payload.value_size());
    std:: cout << std::endl;

  }

  virtual void
  onTimeout(const ndn::Interest& interest)
  {
    std::cout << "Interest timed out " << interest << std::endl;
    exit(0);
  }


protected:
  const ndn::Name m_prefix;
  ndn::Face m_face;
};

int
main(int argc, char** argv)
{
  Consumer c("/hello/world");
  c.start();

  return 0;
}
