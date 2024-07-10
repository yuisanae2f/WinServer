#pragma once

#ifndef JSONMGR_H
#define JSONMGR_H

#include <string>
#include <vector>
#include <unordered_map>

namespace Json {

	enum eType {
		List = '0',
		Obj,
		Dic,
		Str,
		Interface
	};

	template<eType>
	struct cElement;

	using iObj = cElement<Interface>;
	using cObj = cElement<Obj>;
	using cDic = cElement<Dic>;
	using cStr = cElement<Str>;
	using cList = cElement<List>;

	class cReadMgr {
		iObj* raw;
	public:
		union uALL {
			iObj* _interface;
			cObj* classObject;
			cList* classList;
			cDic* classDictionary;
			cStr* classString;
		};

		constexpr cReadMgr(iObj* a = 0) : raw(a) {}
		cReadMgr(std::string);
		~cReadMgr();
		constexpr uALL Union() { return { this->raw }; }
		void Kill();
		void Again(std::string);
		std::string String();

		// we won't think about it from now on.
		constexpr iObj* Abandon() { iObj* r = this->raw; this->raw = 0; return r; }
	private:
		static iObj* Parse(std::string a);
	};

	template<eType e = Interface>
	struct cElement {
		virtual std::string String() = 0;
		virtual eType Type() = 0;
		virtual void Kill();
		virtual ~cElement();

		constexpr cReadMgr::uALL Union() {
			return { this };
		}
	};
	template<>
	struct cElement<Obj> : iObj {
		std::string raw;
		inline cElement() : raw() {}
		virtual std::string String() override;
		virtual eType Type() override;
		virtual void Kill() override;
	};
	template<>
	struct cElement<Str> : cObj {
		inline cElement() : cObj() {}
		virtual std::string String() override;
		virtual eType Type() override;
	};
	template<>
	struct cElement<List> : iObj {
		std::vector<iObj*> vector;
		inline cElement() : vector() {}
		virtual std::string String() override;
		virtual eType Type() override;
		virtual void Kill() override;
	};
	template<>
	struct cElement<Dic> : iObj {
		std::unordered_map<std::string, iObj*> dic;
		inline cElement() : dic() {}
		virtual std::string String() override;
		virtual eType Type() override;
		virtual void Kill() override;
	};

	template<eType e = Interface>
	class cPath {
	protected:
		iObj* obj;
	public:
		virtual ~cPath();
		constexpr cPath(iObj* a = 0) : obj(a) {}
		virtual iObj*& Path();
		virtual eType Type();
		constexpr iObj*& Self() { return obj; }
	};
	template<>
	class cPath<Obj> : public cPath<Interface> {
		virtual eType Type() override;
	};
	template<>
	class cPath<Str> : public cPath<Interface> {
		virtual eType Type() override;
	};
	template<>
	class cPath<List> : public cPath<Interface> {
	public:
		size_t idx;

		constexpr cPath(iObj* a, size_t idx) : 
			cPath<Interface>(a), 
			idx(idx) {}

		virtual iObj*& Path() override;
		virtual eType Type() override;
	};
	template<>
	class cPath<Dic> : public cPath<Interface> {
	public:
		std::string key;

		cPath(iObj* a, std::string _key);

		virtual eType Type() override;
		virtual iObj*& Path() override;
	};

	class cPathMgr {
		iObj* obj;
	public:
		constexpr iObj*& operator()() { return obj; }
		cPathMgr& operator[](std::string);
		cPathMgr& operator[](size_t);
		constexpr cPathMgr(iObj* a = 0) : obj(a) {}
	};
}

#endif