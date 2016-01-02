#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#if 0

using CharType = char;

class SourceIterator
{
public:
  using IndexType = std::uint32_t;

  SourceIterator(IndexType index, CharType value)
  : m_index(index)
  , m_value(value)
  {
  }

  CharType operator*() const
  {
    return m_value;
  }

private:
  IndexType m_index;
  CharType m_value;
};

class InputSource
{
public:

  template <typename T> bool increment_until(const T &t)
  {
    while (!t(*current()) && increment())
    {
    }

    return finished();
  }

  bool increment_then_skip_to_non_white_space()
  {
    increment();
    return increment_until([](CharType t)
    {
      return !isspace(t);
    });
  }

  template <typename T> bool increment_while(const T &t)
  {
    while (increment() && t(current()))
    {
    }

    return finished();
  }

  virtual SourceIterator current() = 0;
  virtual bool increment() = 0;
  virtual bool finished() = 0;
};

class StreamInputSource : public InputSource
{
public:
  StreamInputSource(std::istream &stream)
  : m_chunk_size(128)
  , m_stream(stream)
  , m_index(0)
  , m_finished(false)
  {
    read_more();
  }

  SourceIterator current() override
  {
    return { m_index, m_read_source[m_index] };
  }

  bool increment() override
  {
    ++m_index;
    if (m_index < m_read_source.size())
    {
      return true;
    }

    return read_more();
  }

  bool finished() override
  {
    return m_finished && m_index >= m_read_source.size();
  }

private:
  bool read_more()
  {
    const auto old_size = m_read_source.size();
    m_read_source.resize(old_size + m_chunk_size);

    m_stream.read(m_read_source.data() + old_size, m_chunk_size);
    m_read_source.resize(old_size + m_stream.gcount());
    m_finished = m_stream.eof();
    return !finished();
  }

  const std::size_t m_chunk_size;
  std::istream &m_stream;
  std::vector<CharType> m_read_source;
  SourceIterator::IndexType m_index;
  bool m_finished;
};

class Node;

class AstContainer
{
public:
  template <typename T>T *create()
  {
    auto node = std::make_unique<T>();

    auto val = node.get();
    m_nodes.push_back(std::move(node));
    return val;
  }

  std::vector<std::unique_ptr<Node>> m_nodes;
};

class Node
{
public:
  Node()
  {
  }

  Node(const Node &) = delete;
  Node &operator=(const Node &) = delete;

  virtual void parse_into(InputSource &, AstContainer *container) = 0;
};

template <CharType Start, CharType End, CharType Separator, typename ListItem> class ListNode : public Node
{
public:
  void parse_into(InputSource &src, AstContainer *container) override
  {
    bool started = false;
    bool complete = false;
    bool waiting_for_element = false;
    bool requires_element = false;
    do
    {
      if (waiting_for_element)
      {
        auto created = container->create<ListItem>();
        created->parse_into(src, container);
      }
      else
      {
        src.increment_then_skip_to_non_white_space();
      }

      auto val = src.current();
      switch (*val)
      {
      case Start:
        started = true;
        waiting_for_element = true;
        break;

      case End:
        if (!started)
        {
          throw std::runtime_error("argh. expected start.");
        }
        if (requires_element)
        {
          throw std::runtime_error("argh. separator then end element.");
        }
        complete = true;
        break;

      case Separator:
        if (!started)
        {
          throw std::runtime_error("argh. expected start.");
        }
        waiting_for_element = true;
        requires_element = true;
        break;

      default:
        throw std::runtime_error("argh. unexpected character.");
        break;

      }
    } while (!complete);
  }
};

class SymbolReference : public Node
{
public:
  void parse_into(InputSource &src, AstContainer *container)
  {
    src.increment_then_skip_to_non_white_space();
    src.increment_until([](CharType t) {
      return !isalpha(t) && !isdigit(t);
    });
  }
};

class Expression : public SymbolReference
{
public:
};

class Argument : public Expression
{
public:
};

class ArgumentList : public ListNode<'(', ')', ',', Argument>
{
public:
  
};

int main(int argc, char **argv)
{
#if 0
  if (argc != 2) {
    std::cerr << "Incorrect argument count" << std::endl;
  }

  std::fstream str(argv[1], std::ios::in);
#endif
  std::stringstream str;
  str.str("    (a, b, c)");
  StreamInputSource source(str);

  AstContainer container;
  auto root = container.create<ArgumentList>();
  root->parse_into(source, &container);

  return 1;
}

#endif