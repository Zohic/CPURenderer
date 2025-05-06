#include "CPURENDefines.h"
#include "AttributeAvailability.h"

using namespace cpuRenderBase;

uint8_t AttributeAvailability::AttrListToMask(const bool* list){
	return
		atrr_offsets[ATTR_POS_INDEX] |
		atrr_offsets[ATTR_NORMAL_INDEX] * list[ATTR_NORMAL_INDEX] |
		atrr_offsets[ATTR_TANGENT_INDEX] * list[ATTR_TANGENT_INDEX] |
		atrr_offsets[ATTR_TEXCOORD_INDEX] * list[ATTR_TEXCOORD_INDEX] |
		atrr_offsets[ATTR_COLOR_INDEX] * list[ATTR_COLOR_INDEX];
}

AttributeAvailability::AttributeAvailability() {

}
AttributeAvailability::AttributeAvailability(AttributeAvailability&& old) noexcept {
	attributeMask = old.attributeMask;
	attributeList = old.attributeList;

	old.attributeList.fill(false);
	old.attributeMask = 0;
}

AttributeAvailability& AttributeAvailability::operator=(AttributeAvailability&& old) noexcept {
	attributeMask = old.attributeMask;
	attributeList = old.attributeList;

	old.attributeList.fill(false);
	old.attributeMask = 0;

	return *this;
}

#define AttrFromMask(attr) (bool)((attributeMask & atrr_offsets[attr]) >> attr)

AttributeAvailability::AttributeAvailability(uint8_t mask) : attributeMask(mask) {

	if (!(AttrFromMask(ATTR_POS_INDEX)))
		throw std::logic_error("attribute mask has no POSITION");

	attributeList[ATTR_POS_INDEX] = true;
	attributeList[ATTR_NORMAL_INDEX] = AttrFromMask(ATTR_NORMAL_INDEX);
	attributeList[ATTR_TANGENT_INDEX] = AttrFromMask(ATTR_TANGENT_INDEX);
	attributeList[ATTR_TEXCOORD_INDEX] = AttrFromMask(ATTR_TEXCOORD_INDEX);
	attributeList[ATTR_COLOR_INDEX] = AttrFromMask(ATTR_COLOR_INDEX);


}


AttributeAvailability::AttributeAvailability(const bool* list) {

	if (!list[ATTR_POS_INDEX])
		throw std::logic_error("attribute list has no POSITION");

	attributeMask =
		atrr_offsets[ATTR_POS_INDEX] |
		atrr_offsets[ATTR_NORMAL_INDEX] * list[ATTR_NORMAL_INDEX] |
		atrr_offsets[ATTR_TANGENT_INDEX] * list[ATTR_TANGENT_INDEX] |
		atrr_offsets[ATTR_TEXCOORD_INDEX] * list[ATTR_TEXCOORD_INDEX] |
		atrr_offsets[ATTR_COLOR_INDEX] * list[ATTR_COLOR_INDEX];

	memcpy(attributeList.data(), list, 5ULL * sizeof(bool));
}

void AttributeAvailability::SetAttrAvailability(uint8_t mask) {

	attributeMask = mask;

	if (!(AttrFromMask(ATTR_POS_INDEX)))
		throw std::logic_error("attribute mask has no POSITION");

	attributeList[ATTR_POS_INDEX] = true;
	attributeList[ATTR_NORMAL_INDEX] = AttrFromMask(ATTR_NORMAL_INDEX);
	attributeList[ATTR_TANGENT_INDEX] = AttrFromMask(ATTR_TANGENT_INDEX);
	attributeList[ATTR_TEXCOORD_INDEX] = AttrFromMask(ATTR_TEXCOORD_INDEX);
	attributeList[ATTR_COLOR_INDEX] = AttrFromMask(ATTR_COLOR_INDEX);
}

void AttributeAvailability::SetAttrAvailability(const bool* list) {
	if (!list[ATTR_POS_INDEX])
		throw std::logic_error("attribute mask has no POSITION");

	attributeMask =
		atrr_offsets[ATTR_POS_INDEX] |
		atrr_offsets[ATTR_NORMAL_INDEX] * list[ATTR_NORMAL_INDEX] |
		atrr_offsets[ATTR_TANGENT_INDEX] * list[ATTR_TANGENT_INDEX] |
		atrr_offsets[ATTR_TEXCOORD_INDEX] * list[ATTR_TEXCOORD_INDEX] |
		atrr_offsets[ATTR_COLOR_INDEX] * list[ATTR_COLOR_INDEX];

	memcpy(attributeList.data(), list, 5ULL * sizeof(bool));
}

uint8_t AttributeAvailability::GetAttrMask() const {
	return attributeMask;
}
const bool* AttributeAvailability::GetAttrList() const {
	return attributeList.data();
}
bool AttributeAvailability::HasAttr(size_t attr) const {
	return attributeList[attr];
}
size_t AttributeAvailability::AttributesSize() const {
	return 4ULL +
		HasAttr(ATTR_NORMAL_INDEX) * 3ULL +
		HasAttr(ATTR_TANGENT_INDEX) * 4ULL +
		HasAttr(ATTR_TEXCOORD_INDEX) * 2ULL +
		HasAttr(ATTR_COLOR_INDEX) * 4ULL;
}

void AttributeAvailability::PrintAttributes() const {
	const char* const attrnames[5] = { "POSITION", "NORMAL", "TANGENT", "TEXCOORD", "COLOR" };
	for (int i = 0; i < ATTRIBUTES_NUM; i++)
		printf("%s: %s", attrnames[i], HasAttr(i) ? "YES\n" : "NO\n");
}

#undef AttrFromMask(attr)