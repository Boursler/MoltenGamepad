#ifndef EVENT_CHANGE_H
#define EVENT_CHANGE_H
#include "../output_slot.h"
#include <linux/input.h>
#include <string>
#include "../eventlists/eventlist.h"
#include "../devices/device.h"


#define EVENT_KEY 0
#define EVENT_AXIS 1

#define RANGE 32768
#define ABS_RANGE RANGE

struct mg_ev {
  int64_t value;
};

class input_source;
class event_translator;
class advanced_event_translator;

enum MGType {
  MG_KEY_TRANS,
  MG_REL_TRANS,
  MG_AXIS_TRANS,
  MG_TRANS,
  MG_ADVANCED_TRANS,
  MG_KEY,
  MG_AXIS,
  MG_REL,
  MG_STRING,
  MG_INT,
  MG_FLOAT, //Not implemented...
  MG_SLOT,
  MG_KEYBOARD_SLOT,
  MG_NULL,
};
//Maybe add a varargs or tuple type?

struct MGField {
  MGType type;
  union {
    event_translator* trans;
    advanced_event_translator* adv_trans;
    int key;
    int axis;
    int rel;
    std::string* string;
    int integer;
    float real;
    output_slot* slot;
  };
};

struct MGTransDef {
  std::string identifier;
  std::vector<MGField> fields;
};

//A simple event translator. Takes one input event, and translates it. Essentially just a "pipe".
class event_translator {
public:
  virtual void process(struct mg_ev ev, output_slot* out) {
  }

  void write_out(struct input_event ev, output_slot* out) {
    out->take_event(ev);
  }

  //event translators are passed around via cloning.
  //This isn't just for memory management, but also
  //lets the profile store a translator as a "prototype"
  virtual event_translator* clone() {
    return new event_translator(*this);
  }

  virtual ~event_translator() {};

  event_translator(std::vector<MGField>& fields) {};
  virtual void fill_def(MGTransDef& def) {
    def.identifier = "nothing";
  }
  event_translator() {};
};

//A more complicated event translator. It can request to listen to multiple events.
class advanced_event_translator {
public:
  //Initialize any values needed with this input source
  virtual void init(input_source* source) {};
  //Called when the device's thread is ready for attaching.
  virtual void attach(input_source* source) {};
  //Return true to block the input source's native handling of this event.
  virtual bool claim_event(int id, mg_ev event) {
    return false;
  };
  //Similar to the above, acts as a prototype method.
  virtual advanced_event_translator* clone() {
    return new advanced_event_translator(*this);
  }
  virtual ~advanced_event_translator() {};

  advanced_event_translator(std::vector<MGField>& fields) {};
  advanced_event_translator() {};
  virtual void fill_def(MGTransDef& def) {
    def.identifier = "nothing";
  }
};

#endif