//
// Copyright (C) 2019 OpenSim Ltd.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//
// @author: Zoltan Bojthe
//

#ifndef __INET_SELFDOC_H
#define __INET_SELFDOC_H

namespace inet {

class SelfDoc
{
  protected:
    std::set<std::string> textSet;
  protected:
  public:
    SelfDoc() {}
    ~SelfDoc() noexcept(false);
    void insert(const std::string& text) { textSet.insert(text); }
    static bool notInInitialize() { return true; }
    static bool notInInitialize(const char *methodFmt, ...) { return methodFmt != nullptr && (0 != strcmp(methodFmt, "initialize(%d)")); }
    static const char *enterMethodInfo() { return ""; }
    static const char *enterMethodInfo(const char *methodFmt, ...) { return methodFmt != nullptr ? methodFmt : ""; }

    static std::string kindToStr(int kind, cProperties *properties1, const char *propName1, cProperties *properties2, const char *propName2);
    static std::string val(const char *str);
    static std::string val(const std::string& str) { return val(str.c_str()); }
    static std::string keyVal(const std::string& key, const std::string& value) { return val(key) + " : " + val(value); }
};

extern SelfDoc globalSelfDoc;

#undef Enter_Method
#define Enter_Method(...) \
        auto __from = getSimulation()->getContext(); \
        omnetpp::cMethodCallContextSwitcher __ctx(this); \
        __ctx.methodCall(__VA_ARGS__); \
        if (SelfDoc::notInInitialize(__VA_ARGS__)) { \
            std::ostringstream os; \
            os << "=SelfDoc={ " << SelfDoc::keyVal("module", __from ? __from->getComponentType()->getFullName() : "-=unknown=-") \
               << ", " << SelfDoc::keyVal("action","CALL") \
               << ", " << SelfDoc::val("details") << " : { " \
               << SelfDoc::keyVal("callto", this->getComponentType()->getFullName()) \
               << ", " << SelfDoc::keyVal("func", std::string(opp_typename(typeid(*this))) + "::" + __func__) \
               << ", " << SelfDoc::keyVal("info", SelfDoc::enterMethodInfo(__VA_ARGS__)) \
               << " } }"; \
            globalSelfDoc.insert(os.str()); \
        }

#undef Enter_Method_Silent
#define Enter_Method_Silent(...) \
    auto __from = getSimulation()->getContext(); \
    omnetpp::cMethodCallContextSwitcher __ctx(this); \
    __ctx.methodCallSilent(__VA_ARGS__); \
    if (SelfDoc::notInInitialize(__VA_ARGS__)) { \
        std::ostringstream os; \
       os << "=SelfDoc={ " << SelfDoc::keyVal("module", __from ? __from->getComponentType()->getFullName() : "-=unknown=-") \
          << ", " << SelfDoc::keyVal("action","CALL") \
          << ", " << SelfDoc::val("details") << " : { " \
          << SelfDoc::keyVal("callto", this->getComponentType()->getFullName()) \
          << ", " << SelfDoc::keyVal("func", std::string(opp_typename(typeid(*this))) + "::" + __func__) \
          << ", " << SelfDoc::keyVal("info", SelfDoc::enterMethodInfo(__VA_ARGS__)) \
          << " } }"; \
        globalSelfDoc.insert(os.str()); \
    }

} // namespace inet

#endif // ifndef __INET_SELFDOC_H
