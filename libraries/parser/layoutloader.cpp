#include "layoutloader.hpp"

#include <ui/UILayout.h>

namespace ee {
using Self = LayoutLoader;
using Target = cocos2d::ui::Layout;
using Helper = PropertyHelper<Target>;

const PropertyColor3B Self::Property::BackgroundColor(
    "background_color",
    Helper::makeReader<cocos2d::Color3B>(
        std::mem_fn(&Target::getBackGroundColor)),
    Helper::makeWriter<cocos2d::Color3B>([](Target* node,
                                            const cocos2d::Color3B& value) {
        node->setBackGroundColor(value);
    }));

const PropertyInt Self::Property::BackgroundColorOpacity(
    "background_color_opacity",
    Helper::makeReader<int>(std::mem_fn(&Target::getBackGroundColorOpacity)),
    Helper::makeWriter<int>(std::mem_fn(&Target::setBackGroundColorOpacity)));

const PropertyEnum<Target::BackGroundColorType>
    Self::Property::BackgroundColorType(
        "background_color_type",
        Helper::makeReader<Target::BackGroundColorType>(
            std::mem_fn(&Target::getBackGroundColorType)),
        Helper::makeWriter<Target::BackGroundColorType>(
            std::mem_fn(&Target::setBackGroundColorType)));

const PropertyPoint Self::Property::BackgroundColorVector(
    "background_color_vector",
    Helper::makeReader<cocos2d::Point>(
        std::mem_fn(&Target::getBackGroundColorVector)),
    Helper::makeWriter<cocos2d::Point>(
        std::mem_fn(&Target::setBackGroundColorVector)));

// const PropertyColor3B<Target> BackgroundEndColor("background_end_color",
// std::mem_fn(&Target::getBackGroundEndColor),
// std::mem_fn(&Target::SetBackgroundE)); const PropertyColor3B<Target>
// BackgroundStartColor("background_start_color");

const PropertyRect Self::Property::BackgroundImageCapInsets(
    "background_image_cap_insets",
    Helper::makeReader<cocos2d::Rect>(
        std::mem_fn(&Target::getBackGroundImageCapInsets)),
    Helper::makeWriter<cocos2d::Rect>(
        std::mem_fn(&Target::setBackGroundImageCapInsets)));

const PropertyColor3B Self::Property::BackgroundImageColor(
    "background_image_color",
    Helper::makeReader<cocos2d::Color3B>(
        std::mem_fn(&Target::getBackGroundImageColor)),
    Helper::makeWriter<cocos2d::Color3B>(
        std::mem_fn(&Target::setBackGroundImageColor)));

const PropertyInt Self::Property::BackgroundImageOpacity(
    "background_image_opacity",
    Helper::makeReader<int>(std::mem_fn(&Target::getBackGroundImageOpacity)),
    Helper::makeWriter<int>(std::mem_fn(&Target::setBackGroundImageOpacity)));

// const PropertyString<Target> BackgroundImageName("background_image_name",
// std::mem_fn(&Target::getBackgroundIma));

// const PropertyInt<Target>
// BackgroundImageTexType("background_image_tex_type");

// const PropertyBool<Target>
// BackgroundImageScale9Enabled("background_image_scale_9_enabled");

const PropertyBool Self::Property::ClippingEnabled(
    "clipping_enabled",
    Helper::makeReader<bool>(std::mem_fn(&Target::isClippingEnabled)),
    Helper::makeWriter<bool>(std::mem_fn(&Target::setClippingEnabled)));

const PropertyEnum<Target::ClippingType>
    Self::Property::ClippingType("clipping_type",
                                 Helper::makeReader<Target::ClippingType>(
                                     std::mem_fn(&Target::getClippingType)),
                                 Helper::makeWriter<Target::ClippingType>(
                                     std::mem_fn(&Target::setClippingType)));

const PropertyEnum<Target::Type> Self::Property::LayoutType(
    "layout_type",
    Helper::makeReader<Target::Type>(std::mem_fn(&Target::getLayoutType)),
    Helper::makeWriter<Target::Type>(std::mem_fn(&Target::setLayoutType)));

const std::string Self::ClassName = "_Layout";

Self::LayoutLoader() {}

Self::~LayoutLoader() {}

cocos2d::Node* Self::createNode() const {
    return cocos2d::ui::Layout::create();
}

void Self::loadProperties(cocos2d::Node* node_,
                          const PropertyHandler& handler) const {
    auto node = dynamic_cast<Target*>(node_);
    Super::loadProperties(node, handler);

    handler.loadProperty(Property::BackgroundColor, node);
    handler.loadProperty(Property::BackgroundColorOpacity, node);
    handler.loadProperty(Property::BackgroundColorType, node);
    handler.loadProperty(Property::BackgroundColorVector, node);
    handler.loadProperty(Property::BackgroundImageCapInsets, node);
    handler.loadProperty(Property::BackgroundImageColor, node);
    handler.loadProperty(Property::BackgroundImageOpacity, node);
    handler.loadProperty(Property::ClippingEnabled, node);
    handler.loadProperty(Property::ClippingType, node);
    handler.loadProperty(Property::LayoutType, node);
}

void Self::storeProperties(const cocos2d::Node* node_,
                           PropertyHandler& handler) const {
    auto node = dynamic_cast<const Target*>(node_);
    Super::storeProperties(node, handler);

    handler.storeProperty(Property::BackgroundColor, node);
    handler.storeProperty(Property::BackgroundColorOpacity, node);
    handler.storeProperty(Property::BackgroundColorType, node);
    handler.storeProperty(Property::BackgroundColorVector, node);
    handler.storeProperty(Property::BackgroundImageCapInsets, node);
    handler.storeProperty(Property::BackgroundImageColor, node);
    handler.storeProperty(Property::BackgroundImageOpacity, node);
    handler.storeProperty(Property::ClippingEnabled, node);
    handler.storeProperty(Property::ClippingType, node);
    handler.storeProperty(Property::LayoutType, node);
}

std::string Self::getClassName() const {
    return ClassName;
}

Self* Self::cloneRaw() const {
    return new Self();
}
} // namespace ee
