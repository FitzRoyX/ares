struct CPU : MOS6502, Thread {
  Node::Object node;
  Memory::Writable<n8> ram;

  struct Debugger {
    //debugger.cpp
    auto load(Node::Object) -> void;
    auto instruction() -> void;
    auto interrupt(string_view) -> void;

    struct Memory {
      Node::Debugger::Memory ram;
    } memory;

    struct Tracer {
      Node::Debugger::Tracer::Instruction instruction;
      Node::Debugger::Tracer::Notification interrupt;
    } tracer;
  } debugger;

  auto rate() const -> u32 { return Region::PAL() ? 16 : 12; }

  //cpu.cpp
  auto load(Node::Object) -> void;
  auto unload() -> void;

  auto main() -> void;
  auto step(u32 clocks) -> void;

  auto power(bool reset) -> void;

  //memory.cpp
  auto readBus(n16 address) -> n8;
  auto writeBus(n16 address, n8 data) -> void;

  auto readIO(n16 address) -> n8;
  auto writeIO(n16 address, n8 data) -> void;

  auto readDebugger(n16 address) -> n8 override;

  auto serialize(serializer&) -> void;

  //timing.cpp
  auto read(n16 address) -> n8 override;
  auto write(n16 address, n8 data) -> void override;
  auto lastCycle() -> void override;
  auto cancelNmi() -> void override;
  auto delayIrq() -> void override;
  auto irqPending() -> bool override;
  auto nmi(n16& vector) -> void override;

  auto dmcDMAPending() -> void;
  auto dma(n16 address) -> void;

  auto nmiLine(bool) -> void;
  auto irqLine(bool) -> void;
  auto apuLine(bool) -> void;

//protected:
  struct IO {
    n1  interruptPending;
    n1  nmiPending;
    n1  nmiLine;
    n1  irqLine;
    n1  apuLine;
    n1  oddCycle;
    n1  dmcDMAPending;
    n1  dmcDummyRead;
    n1  oamDMAPending;
    n8  oamDMAPage;
    n8  openBus;
  } io;
};

extern CPU cpu;
