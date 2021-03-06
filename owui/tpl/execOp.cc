#include <olibs/base/assert.hh>
#include <owui/tpl/execOp.hh>
#include <owui/tpl/staticContext.hh>
#include <owui/tpl/dynamicContext.hh>
#include <owui/tpl/tag.hh>
#include <owui/tpl/code.hh>
#include <owui/tpl/templateManager.hh>
#include <sstream>


namespace Owui {
namespace Tpl {


ExecOp::ExecOp(const String& tagName, const OpParametrs& params) :
  m_params(params)
{
  OLIBS_ASSERT(tagName.size() > 0);
  
  m_tagName = tagName;
  if(m_tagName[0] != '/')
    m_tagName = "/" + m_tagName;  
}

void ExecOp::exec(Ostream& os, DynamicContext& dctx) const
{
  const StaticContext& sctx = *TemplateManager::instance()->codeCtx();
  const TagInfo& tagInfo = sctx.tagCatalog().findInfo(m_tagName);
  
  Olibs::Optr<Olibs::Rto::Dynamic> paramsDynamic(new Rto::Dynamic(*tagInfo.m_paramsMeta));
  try
  {
    m_params.fill(*paramsDynamic.get(), dctx);
  }
  catch(Olibs::Rto::HiMeta::FieldNotFoundEx& ex)
  {
    os << m_tagName << " ex: " << ex.what();
    return;
  }  
  
  TagContext tagCtx(paramsDynamic, &m_code.commands(), &dctx);  
  TagLock tagLock(sctx.tagAllocator(), tagName());
  tagLock->draw(os, tagCtx);
}

void ExecOp::print(Ostream& os, const String& prefix) const
{
  // \todo: Добавить распечатку params из Dynamic. Причем сам dynamic должен уметь print
  os << "exec: " << tagName() << "\n";
}


RootNodeCodeAdapter::RootNodeCodeAdapter(Code& code) : 
  m_code(code)
{}

RootNodeCodeAdapter::RootNodeCodeAdapter(const RootNodeCodeAdapter& rhs) :
  m_code(rhs.m_code)
{}

RootNodeCodeAdapter& RootNodeCodeAdapter::operator=(const RootNodeCodeAdapter& rhs)
{
  this->m_code = rhs.m_code;
  return *this;
}

void RootNodeCodeAdapter::attachSubNode(CodeOp* op)
{
  OLIBS_ASSERT(op != 0);
  m_code.put(op);
}


} // namespace Tpl
} // namespace Owui

