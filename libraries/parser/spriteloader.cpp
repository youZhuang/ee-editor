#include "spriteloader.hpp"
#include "nodeinfo.hpp"

#include <2d/CCSprite.h>

namespace ee {
using Self = SpriteLoader;
using Target = cocos2d::Sprite;
using Helper = PropertyHelper<Target>;

const PropertyBlend Self::Property::BlendFunc(
    "blend_func",
    Helper::makeReader<cocos2d::BlendFunc>(std::mem_fn(&Target::getBlendFunc)),
    Helper::makeWriter<cocos2d::BlendFunc>(std::mem_fn(&Target::setBlendFunc)));

const PropertyBool Self::Property::FlippedX(
    "flipped_x", //
    Helper::makeReader<bool>(std::mem_fn(&Target::isFlippedX)),
    Helper::makeWriter<bool>(std::mem_fn(&Target::setFlippedX)));

const PropertyBool Self::Property::FlippedY(
    "flipped_y", //
    Helper::makeReader<bool>(std::mem_fn(&Target::isFlippedY)),
    Helper::makeWriter<bool>(std::mem_fn(&Target::setFlippedY)));

const PropertyBool Self::Property::StretchEnabled(
    "stretch_enabled",
    Helper::makeReader<bool>(std::mem_fn(&Target::isStretchEnabled)),
    Helper::makeWriter<bool>(std::mem_fn(&Target::setStretchEnabled)));

const PropertyString Self::Property::Texture(
    "texture", //
    Helper::makeReader<std::string>([](const Target* node) {
        auto&& handler = NodeInfo::getPropertyHandler(node);
        return handler.getProperty("texture").getString();
    }),
    Helper::makeWriter<std::string>([](Target* node, const std::string& value) {
        auto&& handler = NodeInfo::getPropertyHandler(node);
        handler.setProperty("texture", Value(value));

        // Preserve content size and blend
        // func.
        auto&& contentSize = node->getContentSize();
        auto&& blendFunc = node->getBlendFunc();
        node->setTexture(value);
        node->setContentSize(contentSize);
        node->setBlendFunc(blendFunc);
    }));

const PropertyString Self::Property::SpriteFrame(
    "sprite_frame", //
    Helper::makeReader<std::string>([](const Target* node) {
        auto&& handler = NodeInfo::getPropertyHandler(node);
        return handler.getProperty("sprite_frame").getString();
    }),
    Helper::makeWriter<std::string>([](Target* node, const std::string& value) {
        auto&& handler = NodeInfo::getPropertyHandler(node);
        handler.setProperty("sprite_frame", Value(value));

        // Preserve blend func.
        auto&& blendFunc = node->getBlendFunc();
        node->setSpriteFrame(value);
        node->setBlendFunc(blendFunc);
    }));

const std::string Self::ClassName = "_Sprite";

Self::SpriteLoader() {}

Self::~SpriteLoader() {}

/*
writer.setProperty(NodeLoader::Property::AnchorPoint,
                   cocos2d::Vec2::ANCHOR_MIDDLE);

bool flag = true;
flag &= writer.addProperty(Property::BlendFunc,
                           cocos2d::BlendFunc::ALPHA_PREMULTIPLIED);
flag &= writer.addProperty(Property::FlippedX,
false); flag &=
writer.addProperty(Property::FlippedY, false);
flag &=
writer.addProperty(Property::StretchEnabled,
true); flag &=
writer.addProperty(Property::Texture,
std::string()); Q_ASSERT(flag);
*/

cocos2d::Node* Self::createNode() const {
    return Target::create();
}

void Self::loadProperties(cocos2d::Node* node_,
                          const PropertyHandler& handler) const {
    auto node = dynamic_cast<Target*>(node_);
    Super::loadProperties(node, handler);

    handler.loadProperty(Property::BlendFunc, node);
    handler.loadProperty(Property::FlippedX, node);
    handler.loadProperty(Property::FlippedY, node);
    handler.loadProperty(Property::StretchEnabled, node);
    handler.loadProperty(Property::Texture, node);
    handler.loadProperty(Property::SpriteFrame, node);
}

void Self::storeProperties(const cocos2d::Node* node_,
                           PropertyHandler& handler) const {
    auto node = dynamic_cast<const Target*>(node_);
    Super::storeProperties(node, handler);

    handler.storeProperty(Property::BlendFunc, node);
    handler.storeProperty(Property::FlippedX, node);
    handler.storeProperty(Property::FlippedY, node);
    handler.storeProperty(Property::StretchEnabled, node);
    handler.storeProperty(Property::Texture, node);
    handler.storeProperty(Property::SpriteFrame, node);
}

std::string Self::getClassName() const {
    return ClassName;
}

Self* Self::cloneRaw() const {
    return new Self();
}
} // namespace ee
