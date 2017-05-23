#pragma once
#include "../event_change.h"

class redirect_trans : public event_translator {
public:
  event_translator* trans = nullptr;
  virtual_device* redirected;

  redirect_trans(event_translator* trans, virtual_device* redirected) :  redirected(redirected) {
    this->trans = trans->clone();
  }

  virtual ~redirect_trans() {
    if (trans) delete trans;
  }

  virtual void process(struct mg_ev ev, virtual_device* out);
  virtual void process_recurring(virtual_device* out) const;
  virtual void attach(input_source* source);
  virtual bool wants_recurring_events();

  virtual redirect_trans* clone() {
    return new redirect_trans(trans, redirected);
  }

  static const char* decl;
  redirect_trans(std::vector<MGField>& fields);
  virtual void fill_def(MGTransDef& def);

protected:
  redirect_trans() {};

};
