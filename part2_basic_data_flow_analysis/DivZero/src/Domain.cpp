#include "Domain.h"
#include <stdexcept>

//===----------------------------------------------------------------------===//
// Abstract Domain Implementation
//===----------------------------------------------------------------------===//

/* Add your code here */

#include "Domain.h"
#include <cassert>

using namespace dataflow;

Domain::Domain() : Domain(Value = Domain::MaybeZero) {}

Domain::Domain(Element V) : Value{V} {}

Domain *Domain::add(Domain *E1, Domain *E2) {
  // should never be called on uninitialized values
  assert(E1->Value != Domain::Uninit);
  assert(E2->Value != Domain::Uninit);

  if (E1->Value == Domain::Zero && E2->Value == Domain::Zero) {
    return new Domain(Domain::Zero);
  }

  if (E1->Value == Domain::NonZero && E2->Value == Domain::NonZero) {
    return new Domain(Domain::NonZero);
  }

  return new Domain(Domain::MaybeZero);
}

Domain *Domain::sub(Domain *E1, Domain *E2) {
  // should never be called on uninitialized values
  assert(E1->Value != Domain::Uninit);
  assert(E2->Value != Domain::Uninit);

  if (E1->Value == Domain::Zero && E2->Value == Domain::Zero) {
    return new Domain(Domain::Zero);
  }

  if (E1->Value == Domain::NonZero && E2->Value == Domain::NonZero) {
    return new Domain(Domain::NonZero);
  }

  return new Domain(Domain::MaybeZero);
}

Domain *Domain::mul(Domain *E1, Domain *E2) {
  // should never be called on uninitialized values
  assert(E1->Value != Domain::Uninit);
  assert(E2->Value != Domain::Uninit);

  if (E1->Value == Domain::Zero || E2->Value == Domain::Zero) {
    return new Domain(Domain::Zero);
  }

  if (E1->Value == Domain::NonZero && E2->Value == Domain::NonZero) {
    return new Domain(Domain::NonZero);
  }

  return new Domain(Domain::MaybeZero);
}

Domain *Domain::div(Domain *E1, Domain *E2) {
  // should never be called on uninitialized values
  assert(E1->Value != Domain::Uninit);
  assert(E2->Value != Domain::Uninit);

  // TMP: placeholder error if divisor is zero or maybezero
  assert(E2->Value != Domain::Zero);
  assert(E2->Value != Domain::MaybeZero);

  if (E1->Value == Domain::Zero && E2->Value == Domain::NonZero) {
    return new Domain(Domain::Zero);
  }

  if (E1->Value == Domain::NonZero && E2->Value == Domain::NonZero) {
    return new Domain(Domain::NonZero);
  }

  if (E1->Value == Domain::MaybeZero && E2->Value == Domain::MaybeZero) {
    return new Domain(Domain::MaybeZero);
  }

  throw std::invalid_argument("should be unreachable!");
}

Domain *Domain::join(Domain *E1, Domain *E2) {
  if (E1->Value == Domain::Zero && E2->Value == Domain::Zero) {
    return new Domain(Domain::Zero);
  }

  if (E1->Value == Domain::NonZero && E2->Value == Domain::NonZero) {
    return new Domain(Domain::NonZero);
  }

  return new Domain(Domain::MaybeZero);
}

// E1 <= E2
bool Domain::order(Domain E1, Domain E2) {
  // should never be called on uninitialized values
  assert(E1.Value != Domain::Uninit);
  assert(E2.Value != Domain::Uninit);

  // TODO: UNINIT < (Z, NZ) < MBZ ?
  if (E1.Value == E2.Value) {
    return true;
  }

  if (E2.Value == Domain::MaybeZero) {
    return true;
  }

  return false;
}

void Domain::print(raw_ostream &O) {
  switch (Value) {
  case Uninit:
    O << "Uninit";
    break;
  case NonZero:
    O << "NonZero";
    break;
  case Zero:
    O << "Zero";
    break;
  case MaybeZero:
    O << "MaybeZero";
    break;
  }
}

raw_ostream &operator<<(raw_ostream &O, Domain v) {
  v.print(O);
  return O;
}
