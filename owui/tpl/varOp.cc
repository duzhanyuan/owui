#include <owui/tpl/varOp.hh>
#include <olibs/base/debug.hh>
#include <owui/tpl/tag.hh>
#include <owui/tpl/staticContext.hh>
#include <owui/tpl/dynamicContext.hh>
#include <olibs/rto/query.hh>
#include <olibs/base/assert.hh>


namespace Owui {
namespace Tpl {

VarOp::VarOp(const String& query) : 
  m_query(query)
{
  OLIBS_ASSERT(m_query != "");
};  
  
void VarOp::exec(Ostream& os, DynamicContext& dctx) const
{
  try
  {
    Olibs::Rto::Query query(dctx.data());
    Olibs::Rto::Dynamic::Iterator value = query.query(m_query);
  
    if(!value.isDynamicReference() && !value.isListOfDynamic())
      value.print(os);
    else /*oly temporary*/ //\todo
      os << value.name();

  }
  catch(Olibs::Rto::HiMeta::FieldNotFoundEx& ex)
  {
    os << "ex: " << ex.what();
  }
}

void VarOp::print(Ostream& os, const String& prefix) const
{
  os << prefix << "QUERY:\n" << m_query << "\n\n";
}

TextEmittingOp* VarOp::clone()
{
  return new VarOp(*this);
}


} // namespace Tpl
} // namespace Owui

