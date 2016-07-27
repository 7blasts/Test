<<<<<<< HEAD
#ifndef GX_GLM_UTILS_H
#define GX_GLM_UTILS_H


#include <gx_gdom.h>      //
#include <glm/glm.hpp>
//#include <glm/vec3.hpp>   // glm::vec3
//#include <glm/vec4.hpp>   // glm::vec4
//#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

// All set_xx_from_glm_xxxx Use only data copy. Is only way to publicate some
// calculated glm-value in gx DOM model. But DG invalidate must be called
// separatelly something like emit signals, or calling function:
// "gx::xx* set_to_invalid_all_values_calculated_from_this_one(gx::xx*)"

inline gx::m4* set_m4_from_glm_mat4(gx::m4* dst, const glm::mat4& src){
  memcpy(&dst->data.at(0), glm::value_ptr(src), sizeof(float[16]));
  return dst;
}

// Create set data from ??, and return
inline gx::m4* new_m4_from_glm_mat4(const glm::mat4& src) {
  gx::m4* dst = new gx::m4;
  return set_m4_from_glm_mat4(dst, src);
}

inline glm::mat4& set_glm_mat4_from_m4(glm::mat4& dst, const gx::m4* src) {
  memcpy(glm::value_ptr(dst), &src->data.at(0),  sizeof(float[16]));
  return dst;
}

#endif // GX_GLM_UTILS_H
=======
#ifndef GX_GLM_UTILS_H
#define GX_GLM_UTILS_H


#include <gx_gdom.h>      //
#include <glm/glm.hpp>
//#include <glm/vec3.hpp>   // glm::vec3
//#include <glm/vec4.hpp>   // glm::vec4
//#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

// All set_xx_from_glm_xxxx Use only data copy. Is only way to publicate some
// calculated glm-value in gx DOM model. But DG invalidate must be called
// separatelly something like emit signals, or calling function:
// "gx::xx* set_to_invalid_all_values_calculated_from_this_one(gx::xx*)"

inline gx::m4* set_m4_from_glm_mat4(gx::m4* dst, const glm::mat4& src){
  memcpy(&dst->data.at(0), glm::value_ptr(src), sizeof(float[16]));
  return dst;
}

// Create set data from ??, and return
inline gx::m4* new_m4_from_glm_mat4(const glm::mat4& src) {
  gx::m4* dst = new gx::m4;
  return set_m4_from_glm_mat4(dst, src);
}

inline glm::mat4& set_glm_mat4_from_m4(glm::mat4& dst, const gx::m4* src) {
  memcpy(glm::value_ptr(dst), &src->data.at(0),  sizeof(float[16]));
  return dst;
}

#endif // GX_GLM_UTILS_H
>>>>>>> e80704d2f7f95a5beb6e1e6d387ed2eca1824182
