/*
 *  Copyright (c) 2019 by flomesh.io
 *
 *  Unless prior written consent has been obtained from the copyright
 *  owner, the following shall not be allowed.
 *
 *  1. The distribution of any source codes, header files, make files,
 *     or libraries of the software.
 *
 *  2. Disclosure of any source codes pertaining to the software to any
 *     additional parties.
 *
 *  3. Alteration or removal of any notices in or on the software or
 *     within the documentation included within the software.
 *
 *  ALL SOURCE CODE AS WELL AS ALL DOCUMENTATION INCLUDED WITH THIS
 *  SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION, WITHOUT WARRANTY OF ANY
 *  KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 *  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 *  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "pjs/pjs.hpp"
#include "list.hpp"
#include "inbound.hpp"
#include "input.hpp"

#include <atomic>

namespace pipy {

class ContextDataBase;
class Worker;

//
// Context
//

class Context :
  public pjs::ContextTemplate<Context>,
  public List<Context>::Item
{
public:
  auto id() const -> uint64_t { return m_id; }
  auto data(int i) const -> ContextDataBase* { return m_data->at(i)->as<ContextDataBase>(); }
  auto worker() const -> Worker* { return m_worker; }
  auto inbound() const -> Inbound* { return m_inbound; }

protected:
  Context() : Context(nullptr) {}
  ~Context();

  virtual void finalize() { delete this; }

private:
  typedef pjs::PooledArray<pjs::Ref<pjs::Object>> ContextData;

  Context(
    Context *base,
    Worker *worker = nullptr,
    pjs::Object *global = nullptr,
    ContextData *data = nullptr
  );

  uint64_t m_id;
  Worker* m_worker;
  ContextData* m_data;
  pjs::WeakRef<Inbound> m_inbound;

  static std::atomic<uint64_t> s_context_id;

  friend class pjs::ContextTemplate<Context>;
  friend class Waiter;
  friend class Inbound;
};

//
// ContextDataBase
//

class ContextDataBase : public pjs::ObjectTemplate<ContextDataBase> {
public:
  ContextDataBase(pjs::Str *filename)
    : m_filename(filename) {}

  auto filename() const -> pjs::Str* { return m_filename; }
  auto inbound() const -> Inbound* { return m_context->inbound(); }

protected:
  Context* m_context;
  pjs::Ref<pjs::Str> m_filename;

  friend class Context;
};

} // namespace pipy

#endif // CONTEXT_HPP
