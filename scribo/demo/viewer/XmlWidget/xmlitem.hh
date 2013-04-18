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

#ifndef XMLITEM_H
#define XMLITEM_H

#include <QDomElement>

#include "xmlattributes.hh"

class RegionItem;

class XmlItem
{
    public:
        explicit XmlItem(XmlItem *parent = 0, XmlItem *precItem = 0);
        explicit XmlItem(const QDomElement& node, XmlItem *parent = 0, XmlItem *precItem = 0);
        ~XmlItem();

        void load(const QDomElement& node);

        inline QList<XmlItem *> childs() const;
        inline XmlItem *child(int i) const;
        inline void addChild(XmlItem *child);
        inline XmlItem *parent() const;

        inline RegionItem *regionItem();
        inline void setRegionItem(RegionItem *regionItem);

        inline bool isVisible() const;
        inline void setVisible(bool visible);
        inline void show();
        inline void hide();

        inline QString text() const;
        inline int row() const;
        inline XmlAttributes attributes() const;

    private:
        void init(XmlItem *parent = 0, XmlItem *precItem = 0);

        int row_;
        bool isVisible_;
        QList<XmlItem *> childs_;
        XmlItem *parent_;
        RegionItem *regionItem_;
        QString text_;
        XmlAttributes attributes_;
};

inline QList<XmlItem *> XmlItem::childs() const
{ return childs_; }

inline XmlItem *XmlItem::child(int i) const
{ if(i > -1 && i < childs_.count()) return childs_[i]; return 0; }

inline void XmlItem::addChild(XmlItem *child)
{ childs_ << child; }

inline XmlItem *XmlItem::parent() const
{ return parent_; }

inline RegionItem *XmlItem::regionItem()
{ return regionItem_; }

inline void XmlItem::setRegionItem(RegionItem *regionItem)
{ regionItem_ = regionItem; }

inline bool XmlItem::isVisible() const
{ return isVisible_; }

inline void XmlItem::setVisible(bool visible)
{ isVisible_ = visible; }

inline void XmlItem::show()
{ isVisible_ = true; }

inline void XmlItem::hide()
{ isVisible_ = false; }

inline QString XmlItem::text() const
{ return text_; }

inline int XmlItem::row() const
{ return row_; }

inline XmlAttributes XmlItem::attributes() const
{ return attributes_; }

Q_DECLARE_METATYPE(XmlItem)
Q_DECLARE_METATYPE(XmlItem *)

#endif // XMLITEM_H
