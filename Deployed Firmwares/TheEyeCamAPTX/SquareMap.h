#include <cstdint>
#include <ostream>

#pragma once
typedef struct {
     int16_t x0;
     int16_t x1;
     int16_t y0;
     int16_t y1;
} Rectangle;

class SquareMap {
  private:
  uint16_t iwidth;
  uint16_t iheight;
  uint8_t* values;
  bool rectangle_overlap(int16_t x, int16_t y, uint16_t width, uint16_t height, Rectangle* result);
  public:
  SquareMap(uint8_t* values, uint16_t iwidth, uint16_t iheight);
  ~SquareMap();
  SquareMap* get_square(int16_t x, int16_t y, uint16_t height, uint16_t width);
  SquareMap* reduce(uint16_t width, uint16_t height);
  const uint8_t* getValues() const {return this->values;};
  friend std::ostream& operator<<(std::ostream& os, const SquareMap& square_map);
  uint16_t getWidth() const { return this->iwidth;};
  uint16_t getHeight() const { return this->iheight;};
};
