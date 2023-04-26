#pragma once
#include <memory>

namespace ImOsm {
namespace Rich {
class MarkStorage;

class DistanceCalcWidget {
public:
  DistanceCalcWidget(std::shared_ptr<MarkStorage> storage);
  ~DistanceCalcWidget();

  void paint();

private:
  std::shared_ptr<MarkStorage> _storage;

  struct Ui;
  std::unique_ptr<Ui> _ui;
};
} // namespace Rich
} // namespace ImOsm
