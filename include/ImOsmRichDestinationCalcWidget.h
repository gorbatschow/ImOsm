#pragma once
#include <memory>

namespace ImOsm {
namespace Rich {
class MarkStorage;

class DestinationCalcWidget {
public:
  DestinationCalcWidget(std::shared_ptr<MarkStorage> storage);
  ~DestinationCalcWidget();

  void paint();

private:
  struct Ui;
  std::unique_ptr<Ui> _ui;
  std::shared_ptr<MarkStorage> _storage;
};
} // namespace Rich
} // namespace ImOsm
