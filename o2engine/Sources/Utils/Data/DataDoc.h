#pragma once

#include "Utils/Containers/Array.h"
#include "Utils/String.h"
#include "Utils/Memory/Ptr.h"
#include "Utils/Memory/IObject.h"

namespace o2
{
	class DataNode: public IObject
	{
		friend class DataDoc;

	public:
		typedef Array<Ptr<DataNode>> DataNodesArr;
		typedef DataNode::DataNodesArr::Iterator Iterator;
		typedef DataNode::DataNodesArr::ConstIterator ConstIterator;

	protected:
		WString      mName;
		WString      mData;
		DataNode*    mParent;
		DataNodesArr mChildNodes;

	public:
		DataNode();
		DataNode(const DataNode& other);
		DataNode(const WString& name);
		DataNode(const WString& name, char* value);
		DataNode(const WString& name, wchar_t* value);
		DataNode(const WString& name, bool value);
		DataNode(const WString& name, int value);
		DataNode(const WString& name, float value);
		DataNode(const WString& name, UInt value);
		DataNode(const WString& name, const String& value);
		DataNode(const WString& name, const WString& value);
		virtual ~DataNode();

		DataNode& operator=(const DataNode& other);
		DataNode& operator=(char* value);
		DataNode& operator=(wchar_t* value);
		DataNode& operator=(int value);
		DataNode& operator=(bool value);
		DataNode& operator=(float value);
		DataNode& operator=(UInt value);
		DataNode& operator=(const String& value);
		DataNode& operator=(const WString& value);

		explicit operator wchar_t*() const;
		explicit operator bool() const;
		explicit operator int() const;
		explicit operator float() const;
		explicit operator UInt() const;
		explicit operator String() const;
		explicit operator WString() const;

		Ptr<DataNode> operator[](const WString& nodePath) const;
		bool operator==(const DataNode& other) const;
		bool operator!=(const DataNode& other) const;

		Ptr<DataNode> GetParent() const;

		Ptr<DataNode> GetNode(const WString& nodePath) const;
		Ptr<DataNode> AddNode(const WString& name);
		Ptr<DataNode> AddNode(const Ptr<DataNode>& node);
		bool RemoveNode(const Ptr<DataNode>& node);
		bool RemoveNode(const WString& name);

		WString GetName() const;
		void SetName(const WString& name);

		WString& Data();

		const DataNode::DataNodesArr& GetChildNodes() const;

		Iterator Begin();
		Iterator End();

		ConstIterator Begin() const;
		ConstIterator End() const;

		Iterator begin();
		Iterator end();

		ConstIterator begin() const;
		ConstIterator end() const;
	};

	class DataDoc : public DataNode
	{
	public:
		enum class Format { Xml, JSON, Binary };

	public:
		DataDoc();
		DataDoc(const DataNode& node);
		DataDoc(const WString& fileName);

		DataDoc& operator=(const DataNode& other);

		bool LoadFromFile(const String& fileName);
		bool LoadFromData(const WString& data);

		bool SaveToFile(const String& fileName, Format format = Format::Xml) const;
		WString SaveAsWString(Format format = Format::Xml) const;
	};
}