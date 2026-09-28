#pragma once
#include "sleela_email.h"
#include <string>
namespace sleela {
class EmailFacade {
public:
 explicit EmailFacade(sleela_email_message_t message): message_(message) {}
 bool send(std::string& error) const {
  char buffer[2048] = {};
  int ok = sleela_email_send(&message_, buffer, sizeof(buffer));
  error = buffer;
  return ok != 0;
 }
private:
 sleela_email_message_t message_{};
};
}
