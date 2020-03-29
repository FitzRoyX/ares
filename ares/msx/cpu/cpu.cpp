#include <msx/msx.hpp>

namespace ares::MSX {

CPU cpu;
#include "memory.cpp"
#include "debugger.cpp"
#include "serialization.cpp"

auto CPU::load(Node::Object parent, Node::Object from) -> void {
  node = Node::append<Node::Component>(parent, from, "CPU");
  from = Node::scan(parent = node, from);

  debugger.load(parent, from);
}

auto CPU::unload() -> void {
  node = {};
  debugger = {};
}

auto CPU::main() -> void {
  if(io.irqLine) {
    debugger.interrupt("IRQ");
    irq(1, 0x0038, 0xff);
  }

  debugger.instruction();
  instruction();
}

auto CPU::step(uint clocks) -> void {
  Thread::step(clocks);
  Thread::synchronize();
}

auto CPU::power() -> void {
  Z80::bus = this;
  Z80::power();
  Thread::create(system.colorburst(), {&CPU::main, this});

  r.pc = 0x0000;  //reset vector address

  if(Model::MSX() ) ram.allocate (64_KiB);
  if(Model::MSX2()) ram.allocate(256_KiB);

  slot[0] = {3, 0, {0, 0, 0, 0}};
  slot[1] = {2, 1, {0, 0, 0, 0}};
  slot[2] = {1, 2, {0, 0, 0, 0}};
  slot[3] = {0, 3, {0, 0, 0, 0}};

  io = {};
}

auto CPU::setIRQ(bool line) -> void {
  io.irqLine = line;
}

}