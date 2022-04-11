#include "pch.h"

#include <unreal/Material.h>

#include "MaterialDeserializer.h"

namespace unreal {

auto Combiner::set_property(const Property &property) -> bool {
  if (Material::set_property(property)) {
    return true;
  }

  if (property.name == "Material1") {
    material1.from_property(property, archive);
    return true;
  }

  if (property.name == "Material2") {
    material2.from_property(property, archive);
    return true;
  }

  return false;
}

auto Modifier::set_property(const Property &property) -> bool {
  if (Material::set_property(property)) {
    return true;
  }

  if (property.name == "Material") {
    material.from_property(property, archive);
    return true;
  }

  return false;
}

auto FinalBlend::set_property(const Property &property) -> bool {
  if (Modifier::set_property(property)) {
    return true;
  }

  if (property.name == "FrameBufferBlending") {
    fb_blending = static_cast<FrameBufferBlending>(property.uint8_t_value);
    return true;
  }

  if (property.name == "ZWrite") {
    z_write = property.bool_value();
    return true;
  }

  if (property.name == "ZTest") {
    z_test = property.bool_value();
    return true;
  }

  if (property.name == "AlphaTest") {
    alpha_test = property.bool_value();
    return true;
  }

  if (property.name == "TwoSided") {
    two_sided = property.bool_value();
    return true;
  }

  if (property.name == "AlphaRef") {
    alpha_ref = property.uint8_t_value;
    return true;
  }

  if (property.name == "TreatAsTwoSided") {
    treat_as_two_sided = property.bool_value();
    return true;
  }

  return false;
}

auto BitmapMaterial::set_property(const Property &property) -> bool {
  if (RenderedMaterial::set_property(property)) {
    return true;
  }

  if (property.name == "Format") {
    format = static_cast<TextureFormat>(property.uint8_t_value);
    return true;
  }

  if (property.name == "UBits") {
    u_bits = property.uint8_t_value;
    return true;
  }

  if (property.name == "VBits") {
    v_bits = property.uint8_t_value;
    return true;
  }

  if (property.name == "USize") {
    u_size = property.int32_t_value;
    return true;
  }

  if (property.name == "VSize") {
    v_size = property.int32_t_value;
    return true;
  }

  if (property.name == "UClamp") {
    u_clamp = property.int32_t_value;
    return true;
  }

  if (property.name == "VClamp") {
    v_clamp = property.int32_t_value;
    return true;
  }

  return false;
}

void Palette::deserialize() {
  Object::deserialize();
  archive >> colors;
}

auto operator>>(Archive &archive, Mipmap &mipmap) -> Archive & {
  archive >> mipmap.unknown >> mipmap.data >> mipmap.u_size >> mipmap.v_size >>
      mipmap.u_bits >> mipmap.v_bits;
  return archive;
}

void Texture::deserialize() {
  BitmapMaterial::deserialize();

  MaterialDeserializer deserializer{};
  deserializer.deserialize(archive);

  if (format != TEXF_DXT1 &&  //
      format != TEXF_DXT3 &&  //
      format != TEXF_DXT5 &&  //
      format != TEXF_RGBA8 && //
      format != TEXF_G16) {

    archive >> mips;
    return;
  }

  auto block_size = 8;

  switch (format) {
  case TEXF_DXT1: {
    block_size = 8;
  } break;
  case TEXF_DXT3:
  case TEXF_DXT5: {
    block_size = 16;
  } break;
  case TEXF_G16: {
    block_size = 32;
  } break;
  case TEXF_RGBA8: {
    block_size = 64;
  } break;
  }

  const auto exprected_size = (u_size / 4) * (v_size / 4) * block_size;

  Index size{};

  // Ugly workaround, but seems to work.
  // Search until we found some integer that looks like expected mipmap size in
  // bytes, then read that number of bytes, pretending it's an actual mipmap.
  while (size != exprected_size) {
    archive >> size;

    if (static_cast<std::istream &>(archive).eof()) {
      ASSERT(false, "Unreal", "Unexpected EOF while deserializing texture");
      return;
    }
  }

  Mipmap mip{};
  mip.u_size = u_size;
  mip.v_size = v_size;

  for (auto i = 0; i < size; ++i) {
    std::uint8_t byte;
    archive >> byte;
    mip.data.push_back(byte);
  }

  mips.push_back(mip);
}

auto Texture::set_property(const Property &property) -> bool {
  if (BitmapMaterial::set_property(property)) {
    return true;
  }

  if (property.name == "bAlphaTexture") {
    alpha_texture = property.bool_value();
    return true;
  }

  if (property.name == "bTwoSided") {
    two_sided = property.bool_value();
    return true;
  }

  return false;
}

auto Shader::set_property(const Property &property) -> bool {
  if (RenderedMaterial::set_property(property)) {
    return true;
  }

  if (property.name == "Diffuse") {
    diffuse.from_property(property, archive);
    return true;
  }

  if (property.name == "OutputBlending") {
    output_blending = static_cast<OutputBlending>(property.uint8_t_value);
    return true;
  }

  if (property.name == "AlphaTest") {
    alpha_test = property.bool_value();
    return true;
  }

  if (property.name == "AlphaRef") {
    alpha_ref = property.uint8_t_value;
    return true;
  }

  if (property.name == "TreatAsTwoSided") {
    treat_as_two_sided = property.bool_value();
    return true;
  }

  if (property.name == "TwoSided") {
    two_sided = property.bool_value();
    return true;
  }

  return false;
}

} // namespace unreal
