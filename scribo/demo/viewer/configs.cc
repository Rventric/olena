// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.

#include "configs.hh"

Configs::Configs()
  : QSettings("olena-scribo", "gui")
{
}

Configs *Configs::getInstance()
{
  static Configs *conf = new Configs(); return conf;
}

bool Configs::preprocessingSubsample()
{
  return value("preprocessing/subsample", false).toBool();
}

void Configs::setPreprocessingSubsample(bool b)
{
  setValue("preprocessing/subsample", b);
}

bool Configs::preprocessingRemoveBg()
{
  return value("preprocessing/remove_bg", false).toBool();
}

void Configs::setPreprocessingRemoveBg(bool b)
{
  setValue("preprocessing/remove_bg", b);
}

bool Configs::preprocessingDeskew()
{
  return value("preprocessing/deskew", false).toBool();
}

void Configs::setPreprocessingDeskew(bool b)
{
  setValue("preprocessing/deskew", b);
}

bool Configs::preprocessingRemoveNoise()
{
  return value("preprocessing/remove_noise", true).toBool();
}

void Configs::setPreprocessingRemoveNoise(bool b)
{
  setValue("preprocessing/remove_noise", b);
}

int Configs::preprocessingBinAlgo()
{
  return value("preprocessing/bin_algo",
		scribo::toolchain::internal::SauvolaMs).toInt();
}

void Configs::setPreprocessingBinAlgo(int algo)
{
  setValue("preprocessing/bin_algo", algo);
}

int Configs::segmentationFindSeps()
{
  return value("segmentation/find_seps", Separator::Both).toInt();
}

void Configs::setSegmentationFindSeps(int seps)
{
  setValue("segmentation/find_seps", seps);
}

bool Configs::ocrEnabled()
{
  return value("ocr/enabled", true).toBool();
}

void Configs::setOcrEnabled(bool b)
{
  setValue("ocr/enabled", b);
}

QString Configs::ocrLanguage()
{
  return value("ocr/language", 0).toString();
}

void Configs::setOcrLanguage(const QString& lang)
{
  setValue("ocr/language", lang);
}

bool Configs::generalSaveXmlEnabled()
{
  return value("general/save_xml/enabled", true).toBool();
}

void Configs::setGeneralSaveXmlEnabled(bool b)
{
  setValue("general/save_xml/enabled", b);
}

bool Configs::generalSaveXmlSameDir()
{
  return value("general/save_xml/same_dir", true).toBool();
}

void Configs::setGeneralSaveXmlSameDir(bool b)
{
  setValue("general/save_xml/same_dir", b);
}

bool Configs::generalSaveXmlCustomDir()
{
  return value("general/save_xml/custom_dir", false).toBool();
}

void Configs::setGeneralSaveXmlCustomDir(bool b)
{
  setValue("general/save_xml/custom_dir", b);
}

QString Configs::generalSaveXmlCustomDirPath()
{
  return value("general/save_xml/custom_dir_path",
		QDir::tempPath()).toString();
}

void Configs::setGeneralSaveXmlCustomDirPath(const QString& path)
{
  setValue("general/save_xml/custom_dir_path", path);
}
