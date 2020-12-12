// Copyright (c) Facebook, Inc. and its affiliates.
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef ESP_GFX_FISHEYESHADER_H_
#define ESP_GFX_FISHEYESHADER_H_

#include <Corrade/Containers/EnumSet.h>
#include <Corrade/Utility/Macros.h>
#include <Magnum/GL/AbstractShaderProgram.h>
#include <Magnum/GL/CubeMapTexture.h>
#include <Magnum/Shaders/Generic.h>

#include "esp/core/esp.h"

namespace esp {
namespace gfx {

// Interface class for various fisheye camera shaders, such as "Double Sphere
// Camera", "Field-of-View Camera", etc.
class FisheyeShader : public Magnum::GL::AbstractShaderProgram {
 public:
  enum : Magnum::UnsignedInt {
    /**
     * Color shader output. @ref shaders-generic "Generic output",
     * present always. Expects three- or four-component floating-point
     * or normalized buffer attachment.
     */
    ColorOutput = Magnum::Shaders::Generic3D::ColorOutput,

    // TODO
    /**
     * Object ID shader output. @ref shaders-generic "Generic output",
     * present only if @ref Flag::ObjectId is set. Expects a
     * single-component unsigned integral attachment. Writes the value
     * set in @ref setObjectId() there.
     */
    // ObjectIdOutput = Magnum::Shaders::Generic3D::ObjectIdOutput,
  };

  /**
   * @brief Flag
   *
   * @see @ref Flags, @ref flags()
   */
  enum class Flag : Magnum::UnsignedShort {
    /**
     * cubemap color texture
     */
    ColorTexture = 1 << 0,
    // TODO
    // DepthTexture = 1 << 1,
    // ObjectIdTexture = 1 << 2,
  };

  typedef Corrade::Containers::EnumSet<Flag> Flags;

  /**
   * @brief constructor
   */
  explicit FisheyeShader(Flags flags = {Flag::ColorTexture});

  virtual ~FisheyeShader(){};

  /** @brief Flags */
  Flags flags() const { return flags_; }

  /*
  template <typename... DrawableGroupArgs>
  gfx::DrawableGroup* createDrawableGroup(std::string id,
                                          DrawableGroupArgs&&... args) {
    auto inserted = drawableGroups_.emplace(
        std::piecewise_construct, std::forward_as_tuple(std::move(id)),
        std::forward_as_tuple(std::forward<DrawableGroupArgs>(args)...));
        */

  virtual FisheyeShader& bindColorTexture(
      CORRADE_UNUSED Magnum::GL::CubeMapTexture& texture);
  // virtual FisheyeShader& bindDepthTexture(Magnum::GL::Texture2D& texture);
  // virtual FisheyeShader& bindObjectIdTexture(Magnum::GL::Texture2D&
  // texture);

 protected:
  Flags flags_;

  // ======= uniforms =======
  // it hurts the performance to call glGetUniformLocation() every frame due
  // to string operations. therefore, cache the locations in the constructor
  // using the function @ref cacheUniforms()

  // common material uniforms

  /**
   * @brief cache the uniform locations
   * NOTE: Subclass must implement this function, and call it at the end of
   * the constructor
   */
  virtual void cacheUniforms() = 0;

  /**
   * @brief set texture binding points in the shader
   * NOTE: Subclass must implement this function, and call it at the end of
   * the constructor
   */
  virtual void setTextureBindingPoints() = 0;
};
CORRADE_ENUMSET_OPERATORS(FisheyeShader::Flags)
}  // namespace gfx
}  // namespace esp
#endif
