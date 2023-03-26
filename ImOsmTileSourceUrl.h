#pragma once
#include "ImOsmTileSourceAsync.h"
#include <curl/curl.h>
#include <ostream>
#include <sstream>

namespace ImOsm {
class TileSourceUrl : public TileSourceAsync {
public:
  TileSourceUrl();
  TileSourceUrl(const std::string &userAgent);
  virtual ~TileSourceUrl() = default;

protected:
  virtual bool receiveTile(int z, int x, int y,
                           std::vector<std::byte> &blob) override;
  virtual std::string makeUrl(int z, int x, int y) = 0;

private:
  static size_t onPullResponse(void *data, size_t size, size_t nmemb,
                               void *userp);

  std::string _userAgent{"curl"};
};
} // namespace ImOsm
