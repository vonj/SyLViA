#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <iostream>
#include <IceException.h>
#include "FileName.h"

using namespace std;
using ice::IceException;

void FileName::setPath(const std::string& n)
{
  // set parts of n to path
  // includes filename!
  if (n.size() == 0)
    throw IceException("FileName::setPath", "empty pathname");
  absolute = n[0] == pathdel;
  path.clear();
  unsigned int i = 0;
  string thisPart;
  while (i < n.size())
    {
      if (n[i] != pathdel)
        thisPart += n[i];
      else
        {
          if (!thisPart.empty())
            path.push_back(thisPart);
          thisPart.clear();
        }
      ++i;
    }
  if (!thisPart.empty())
    path.push_back(thisPart);
}

FileName::FileName(const std::string& n)
{
  if (n.size() == 0)
    throw IceException("FileName", "empty filename");

  setPath(n);

  // last path component is name
  if (path.size() > 0)
    {
      // last part of path is (file-)name
      name = path.back();
      path.pop_back();

      size_t lastExtensionDelimiterPosition = name.rfind(".");
      if (lastExtensionDelimiterPosition == std::string::npos)
        extension = "";
      else
        {
          extension = name.substr(lastExtensionDelimiterPosition + 1);
          name.resize(lastExtensionDelimiterPosition);
        }
    }
}

void FileName::setName(const string& n)
{
  for (unsigned int i = 0; i < n.size(); ++i)
    if (n[i] == pathdel)
      throw IceException("Filename::setName", "path delimiter in name");
  name = n;
}

void FileName::setExtension(const string& n)
{
  for (unsigned int i = 0; i < n.size(); ++i)
    {
      if (n[i] == extdel)
        throw IceException("Filename::setExtension", "extension delimiter in extension");
      if (n[i] == pathdel)
        throw IceException("Filename::setExtension", "path delimiter in extension");
    }
  extension = n;
}

FileName::FileName(const std::string& path,
                   const std::string& name,
                   const std::string& ext)
{
  setPath(path);
  setName(name);
  setExtension(ext);
}

std::string FileName::getFullName() const
{
  std::string res = getPath();
  if (res != "/" && !res.empty())
    res += '/';
  res += name;
  if (!extension.empty())
    res += extdel + extension;
  return res;
}

FileName::operator string() const
{
  return getFullName();
}

std::string FileName::getPath() const
{
  string res;
  if (absolute)
    res += '/';
  if (path.size() > 0)
    {
      for (unsigned int i = 0; i < path.size() - 1; ++i)
        res += path[i] + '/';
      res += path.back();
    }
  return res;
}
