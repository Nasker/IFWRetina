#include <cstdint>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ostream>
#include <iostream>
#include "SquareMap.h"
#include "Arduino.h"

static const uint8_t empty_fill = 255;

static int16_t min(int16_t a, int16_t b){
  if(a <= b) return a;
  return b;
};

static int16_t max(int16_t a, int16_t b){
  if(a >= b) return a;
  return b;
};

std::ostream& operator<<(std::ostream& os, const SquareMap& square_map) {
  auto values = square_map.getValues();
  os<<"Size: " << square_map.getHeight() <<"x"<< square_map.getWidth()<<std::endl;
  for(uint32_t vindex = 0; vindex < square_map.getHeight(); vindex++) {
    os<<"Line: " << vindex<<" ";
    for(uint32_t hindex = 0; hindex < square_map.getWidth(); hindex++) {
      os<<"Value:"<<(uint16_t) values[vindex*square_map.getWidth() + hindex]<<" ";
    }
    os<<std::endl;
  }
  return os;
};

SquareMap::SquareMap(uint8_t* values, uint16_t iwidth, uint16_t iheight) {
  this->iwidth = iwidth;
  this->iheight = iheight;
  //Serial.printf("iwidth: %d iheight: %d\n", iwidth, iheight);
  this->values = values; 
  //Serial.printf("Created!\n");
  //memcpy((uint8_t*) this->values, values, iwidth*iheight);
  //Serial.printf("Done!\n");
};

SquareMap::~SquareMap(){
  delete[] this->values;
};

bool SquareMap::rectangle_overlap(int16_t x, int16_t y, uint16_t width, uint16_t height, Rectangle* result) {
  int16_t image_x0 = 0;
  int16_t image_x1 = this->iwidth;
  int16_t image_y0 = 0;
  int16_t image_y1 = this->iheight;
  int16_t selected_x0 = x;
  int16_t selected_x1 = x + width;
  int16_t selected_y0 = y;
  int16_t selected_y1 = y + height;
  result->x0 = max(image_x0, selected_x0);
  result->y0 = max(image_y0, selected_y0);
  result->x1 = min(image_x1, selected_x1);
  result->y1 = min(image_y1, selected_y1);
  if((result->x0 > result->x1) || (result->y0 > result->y1)) return false;
  return true;
};

SquareMap* SquareMap::get_square(int16_t x, int16_t y, uint16_t width, uint16_t height) {
  uint8_t* result = new uint8_t [width*height];
  memset(result, empty_fill, width*height);
  uint32_t x_size = 0;
  uint32_t x_origin = 0;
  uint32_t x_destination = 0;
  Rectangle overlap_result;
  if(width > this->iwidth) width = this->iwidth;
  if(height > this->iheight) height = this->iheight;
  if(!rectangle_overlap(x, y, height, width, &overlap_result)) {
    Serial.printf("No overlap!\n");
    return this;
  }
  
  //printf("Overlap Rectangle: x0: %d y0: %d x1: %d y1: %d\n", overlap_result.x0, overlap_result.y0, overlap_result.x1, overlap_result.y1);
  x_size = abs(overlap_result.x1) - abs(overlap_result.x0);
  //x_destination = overlap_result.y0*width;
  if(x < 0) x_destination += abs(x); 
  for(int16_t h_height = overlap_result.y0; h_height < overlap_result.y1 ; h_height++) {
    x_origin = (h_height*iwidth) + overlap_result.x0;
    memcpy(result + x_destination, this->values + x_origin, x_size);
    x_destination += width;
    //Serial.printf("Copying line: %x - %d\n", *(this->values + x_origin), x_origin);
  }
  return new SquareMap(result, width, height);
};

SquareMap* SquareMap::reduce(uint16_t width, uint16_t height) {
  //Resizing Begins
  uint16_t final_width = width;
        uint16_t final_height = height;
        uint32_t final_size = final_width*final_height;
  uint16_t aux_width = (this->iwidth/final_width);
  uint16_t aux_height = (this->iheight/final_height);
  //printf("%d/%d\n", this->iwidth, final_width);
        uint32_t aux_size = aux_width*aux_height;
  uint16_t steps_x = (this->iwidth/aux_width);
  uint16_t steps_y = (this->iheight/aux_height);
  uint8_t *final_result = static_cast<uint8_t*>(malloc(sizeof(uint8_t)*final_size));
  uint16_t next_step_x, next_step_y = 0;
  for(uint16_t step_y = 0; step_y < steps_x; step_y++) {
    next_step_x = 0;
    for(uint16_t step_x = 0; step_x < steps_x; step_x++) {
      //Serial.printf("reduce rectangle: %d %d %d %d\n", next_step_x, next_step_y, aux_height, aux_width);
      auto aux_map = this->get_square(next_step_x, next_step_y, aux_width, aux_height);
      //std::cout<<*aux_map;
      next_step_x += aux_width;
      float point_result = 0;
      auto aux_matrix = aux_map->getValues();
      for(uint32_t index = 0; index<aux_size; index++) {
        point_result += ((aux_matrix[index] - point_result) / (index + 1));
        //Serial.printf("value: %d %f\n", aux_matrix[index], point_result);
      }
      final_result[(step_y*final_width) + step_x] = point_result;
      delete aux_map;
    }
    next_step_y += aux_height;
  }
  return new SquareMap(final_result, width, height);
};
