#pragma once
#include <vector>
#include <glm/glm.hpp>

namespace shb{
class sVertex{
 public:
  glm::vec3 _position;
  glm::vec3 _color;
};


class sVBO{
 public:


  
  std::vector<sVertex> vertices;

  void fillVBO(){
     
   for(int i =0; i<=3; ++i){
    sVertex vertex;
        vertex._color = glm::vec3{130.f,231.f,131.f};
        vertex._position = glm::vec3{244.f,234.f,234.f};
        vertices.push_back(vertex);
    
   }
  }

};
} //namespace shb