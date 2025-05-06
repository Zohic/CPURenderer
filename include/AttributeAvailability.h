#pragma once

#include <array>
namespace cpuRenderBase {
	class AttributeAvailability {
	protected:
		uint8_t attributeMask = 0;
		std::array<bool, 5> attributeList = { false, false, false, false, false };

		AttributeAvailability(const AttributeAvailability& old) = delete;
		AttributeAvailability& operator=(const AttributeAvailability& old) = delete;

	public:

		static uint8_t AttrListToMask(const bool* list);

		AttributeAvailability();
		AttributeAvailability(AttributeAvailability&& old) noexcept;
		AttributeAvailability& operator=(AttributeAvailability&& old) noexcept;

		AttributeAvailability(uint8_t mask);
		AttributeAvailability(const bool* list);

		void SetAttrAvailability(uint8_t mask);
		void SetAttrAvailability(const bool* list);

		uint8_t GetAttrMask() const;
		const bool* GetAttrList() const;
		bool HasAttr(size_t attr) const;
		size_t AttributesSize() const;
		void PrintAttributes() const;

	};
}
