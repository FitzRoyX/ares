Gamepad::Gamepad(Node::Port parent) {
  node = parent->append<Node::Peripheral>("Gamepad");

  up     = node->append<Node::Input::Button>("Up");
  down   = node->append<Node::Input::Button>("Down");
  left   = node->append<Node::Input::Button>("Left");
  right  = node->append<Node::Input::Button>("Right");
  b      = node->append<Node::Input::Button>("B");
  a      = node->append<Node::Input::Button>("A");
  select = node->append<Node::Input::Button>("Select");
  start  = node->append<Node::Input::Button>("Start");
}

auto Gamepad::data() -> n3 {
  if(latched == 1) {
    platform->input(a);
    return a->value();
  }

  switch(counter++) {
  case 0: return a->value();
  case 1: return b->value();
  case 2: return select->value();
  case 3: return start->value();
  case 4: return upLatch;
  case 5: return downLatch;
  case 6: return leftLatch;
  case 7: return rightLatch;
  }

  counter = 8;
  return 1;
}

auto Gamepad::latch(n1 data) -> void {
  if(latched == data) return;
  latched = data;
  counter = 0;

  if(latched == 0) {
    platform->input(a);
    platform->input(b);
    platform->input(select);
    platform->input(start);
    platform->input(up);
    platform->input(down);
    platform->input(left);
    platform->input(right);

    if(!(up->value() & down->value())) {
      yHold = 0, upLatch = up->value(), downLatch = down->value();
    } else if(!yHold) {
      yHold = 1, swap(upLatch, downLatch);
    }

    if(!(left->value() & right->value())) {
      xHold = 0, leftLatch = left->value(), rightLatch = right->value();
    } else if(!xHold) {
      xHold = 1, swap(leftLatch, rightLatch);
    }
  }
}

auto Gamepad::serialize(serializer& s) -> void {
  s(latched);
  s(counter);

  s(yHold);
  s(upLatch);
  s(downLatch);
  s(xHold);
  s(leftLatch);
  s(rightLatch);
}

