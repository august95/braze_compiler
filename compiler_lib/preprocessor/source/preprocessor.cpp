
#include "../../source/pch.h"
#include "../preprocessor.h"
preProcessor::preProcessor()
  :m_enabled(1)
{

}

int preProcessor::startPreProcessor()
{
  if (!m_enabled)
  {
    m_tokens_pre_processed = m_tokens_original;
    return 0;
  }
  return 0;
}
