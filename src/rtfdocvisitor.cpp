/******************************************************************************
 *
 * 
 *
 *
 * Copyright (C) 1997-2002 by Dimitri van Heesch.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation under the terms of the GNU General Public License is hereby 
 * granted. No representations are made about the suitability of this software 
 * for any purpose. It is provided "as is" without express or implied warranty.
 * See the GNU General Public License for more details.
 *
 * Documents produced by Doxygen are derivative works derived from the
 * input used in their production; they are not affected by this license.
 *
 */

#include "rtfdocvisitor.h"
#include "docparser.h"
#include "language.h"
#include "doxygen.h"
#include "outputgen.h"
#include "code.h"
#include "dot.h"
#include "util.h"
#include "rtfstyle.h"

RTFDocVisitor::RTFDocVisitor(QTextStream &t,BaseCodeDocInterface &ci) 
  : m_t(t), m_ci(ci), m_insidePre(FALSE), m_hide(FALSE), m_indentLevel(0) 
{
}

QCString RTFDocVisitor::getStyle(const char *name)
{
  QCString n;
  n.sprintf("%s%d",name,m_indentLevel);
  StyleData *sd = rtf_Style[n];
  ASSERT(sd!=0);
  return sd->reference;
}

  //--------------------------------------
  // visitor functions for leaf nodes
  //--------------------------------------

void RTFDocVisitor::visit(DocWord *w)
{
  if (m_hide) return;
  filter(w->word());
}

void RTFDocVisitor::visit(DocLinkedWord *w)
{
  if (m_hide) return;
  startLink(w->ref(),w->file(),w->anchor());
  filter(w->word());
  endLink(w->ref());
}

void RTFDocVisitor::visit(DocWhiteSpace *w)
{
  if (m_hide) return;
  if (m_insidePre)
  {
    m_t << w->chars();
  }
  else
  {
    m_t << " ";
  }
}

void RTFDocVisitor::visit(DocSymbol *s)
{
  if (m_hide) return;
  switch(s->symbol())
  {
    case DocSymbol::BSlash:  m_t << "\\\\"; break;
    case DocSymbol::At:      m_t << "@"; break;
    case DocSymbol::Less:    m_t << "<"; break;
    case DocSymbol::Greater: m_t << ">"; break;
    case DocSymbol::Amp:     m_t << "&"; break;
    case DocSymbol::Dollar:  m_t << "$"; break;
    case DocSymbol::Hash:    m_t << "#"; break;
    case DocSymbol::Percent: m_t << "%"; break;
    case DocSymbol::Copy:    m_t << "\251"; break;
    case DocSymbol::Apos:    m_t << "'"; break;
    case DocSymbol::Quot:    m_t << "\""; break;
    case DocSymbol::Uml:     switch(s->letter())
                             {
                               case 'A' : m_t << '\304'; break;
                               case 'E' : m_t << '\313'; break;
                               case 'I' : m_t << '\317'; break;
                               case 'O' : m_t << '\326'; break;
                               case 'U' : m_t << '\334'; break;
                               case 'Y' : m_t << 'Y';    break;
                               case 'a' : m_t << '\344'; break;
                               case 'e' : m_t << '\353'; break;
                               case 'i' : m_t << '\357'; break;
                               case 'o' : m_t << '\366'; break;
                               case 'u' : m_t << '\374'; break;
                               case 'y' : m_t << '\377'; break;
                               default: m_t << '?'; break;
                             }
                             break;
    case DocSymbol::Acute:   switch(s->letter())
                             {
                               case 'A' : m_t << '\301'; break;
                               case 'E' : m_t << '\311'; break;
                               case 'I' : m_t << '\315'; break;
                               case 'O' : m_t << '\323'; break;
                               case 'U' : m_t << '\332'; break;
                               case 'Y' : m_t << '\335'; break;
                               case 'a' : m_t << '\341'; break;
                               case 'e' : m_t << '\351'; break;
                               case 'i' : m_t << '\355'; break;
                               case 'o' : m_t << '\363'; break;
                               case 'u' : m_t << '\372'; break;
                               case 'y' : m_t << '\375'; break;
                               default: m_t << '?'; break;
                             }
                             break;
    case DocSymbol::Grave:   switch(s->letter())
                             {
                               case 'A' : m_t << '\300'; break;
                               case 'E' : m_t << '\310'; break;
                               case 'I' : m_t << '\314'; break;
                               case 'O' : m_t << '\322'; break;
                               case 'U' : m_t << '\331'; break;
                               case 'a' : m_t << '\340'; break;
                               case 'e' : m_t << '\350'; break;
                               case 'i' : m_t << '\354'; break;
                               case 'o' : m_t << '\362'; break;
                               case 'u' : m_t << '\371'; break;
                               default: m_t << '?'; break;
                             }
                             break;
    case DocSymbol::Circ:    switch(s->letter())
                             {
                               case 'A' : m_t << '\302'; break;
                               case 'E' : m_t << '\312'; break;
                               case 'I' : m_t << '\316'; break;
                               case 'O' : m_t << '\324'; break;
                               case 'U' : m_t << '\333'; break;
                               case 'a' : m_t << '\342'; break;
                               case 'e' : m_t << '\352'; break;
                               case 'i' : m_t << '\356'; break;
                               case 'o' : m_t << '\364'; break;
                               case 'u' : m_t << '\373'; break;
                               default: m_t << '?'; break;
                             }
                             break;
    case DocSymbol::Tilde:   switch(s->letter())
                             {
                               case 'A' : m_t << '\303'; break;
                               case 'N' : m_t << '\321'; break;
                               case 'O' : m_t << '\325'; break;
                               case 'a' : m_t << '\343'; break;
                               case 'n' : m_t << '\361'; break;
                               case 'o' : m_t << '\365'; break;
                               default: m_t << '?'; break;
                             }
                             break;
    case DocSymbol::Cedil:   switch(s->letter())
                             {
                               case 'C' : m_t << '\307'; break;
                               case 'c' : m_t << '\347'; break;
                               default: m_t << '?'; break;
                             }
                             break;
    case DocSymbol::Ring:    switch(s->letter())
                             {
                               case 'A' : m_t << '\305'; break;
                               case 'a' : m_t << '\345'; break;
                               default: m_t << '?'; break;
                             }
                             break;
    case DocSymbol::Szlig:   m_t << "\337"; break;
    case DocSymbol::Nbsp:    m_t << "\\~ "; break;
    default:
                             printf("Error: unknown symbol found\n");
  }
}

void RTFDocVisitor::visit(DocURL *u)
{
  if (m_hide) return;
  if (Config_getBool("RTF_HYPERLINKS"))
  {
    m_t << "{\\field {\\*\\fldinst { HYPERLINK  \\\\l \"";
    m_t << u->url();
    m_t << "\" }{}";
    m_t << "}{\\fldrslt {\\cs37\\ul\\cf2 ";
    filter(u->url());
    m_t << "}}}" << endl;
  }
  else
  {
    m_t << "{\\f2 ";
    filter(u->url());
    m_t << "}";
  }
}

void RTFDocVisitor::visit(DocLineBreak *)
{
  if (m_hide) return;
  m_t << "\\par\n";
}

void RTFDocVisitor::visit(DocHorRuler *)
{
  if (m_hide) return;
  m_t << "{\\pard\\widctlpar\\brdrb\\brdrs\\brdrw5\\brsp20 \\adjustright \\par}" << endl;
}

void RTFDocVisitor::visit(DocStyleChange *s)
{
  if (m_hide) return;
  switch (s->style())
  {
    case DocStyleChange::Bold:
      if (s->enable()) m_t << "{\\b ";      else m_t << "} ";
      break;
    case DocStyleChange::Italic:
      if (s->enable()) m_t << "{\\i ";     else m_t << "} ";
      break;
    case DocStyleChange::Code:
      if (s->enable()) m_t << "{\\f2 ";   else m_t << "} ";
      break;
    case DocStyleChange::Subscript:
      if (s->enable()) m_t << "{\\sub ";    else m_t << "} ";
      break;
    case DocStyleChange::Superscript:
      if (s->enable()) m_t << "{\\super ";    else m_t << "} ";
      break;
    case DocStyleChange::Center:
      if (s->enable()) m_t << "{\\qc "; else m_t << "} ";
      break;
    case DocStyleChange::Small:
      if (s->enable()) m_t << "{\\sub ";  else m_t << "} ";
      break;
  }
}

void RTFDocVisitor::visit(DocVerbatim *s)
{
  if (m_hide) return;
  switch(s->type())
  {
    case DocVerbatim::Code: // fall though
      m_t << "{" << endl;
      m_t << "\\par" << endl;
      m_t << rtf_Style_Reset << getStyle("CodeExample");
      parseCode(m_ci,s->context(),s->text(),FALSE,0);
      m_t << "\\par" << endl; 
      m_t << "}" << endl;
      break;
    case DocVerbatim::Verbatim: 
      m_t << "{" << endl;
      m_t << "\\par" << endl;
      m_t << rtf_Style_Reset << getStyle("CodeExample");
      m_t << s->text();
      m_t << "\\par" << endl; 
      m_t << "}" << endl;
      break;
    case DocVerbatim::HtmlOnly: 
      /* nothing */ 
      break;
    case DocVerbatim::LatexOnly: 
      /* nothing */
      break;
  }
}

void RTFDocVisitor::visit(DocAnchor *anc)
{
  if (m_hide) return;
  m_t << "\\label{" << anc->anchor() << "}" << endl;
  if (!anc->file().isEmpty() && Config_getBool("PDF_HYPERLINKS")) 
  {
    m_t << "\\hypertarget{" << anc->file() << "_" << anc->anchor() 
      << "}{}" << endl;
  }    
}

void RTFDocVisitor::visit(DocInclude *inc)
{
  if (m_hide) return;
  switch(inc->type())
  {
    case DocInclude::Include: 
      m_t << "\n\n\\footnotesize\\begin{verbatim}"; 
      parseCode(m_ci,inc->context(),inc->text(),FALSE,0);
      m_t << "\\end{verbatim}\\normalsize" << endl; 
      break;
    case DocInclude::DontInclude: 
      break;
    case DocInclude::HtmlInclude: 
      break;
    case DocInclude::VerbInclude: 
      m_t << "\n\n\\footnotesize\\begin{verbatim}"; 
      m_t << inc->text();
      m_t << "\\end{verbatim}\\normalsize" << endl; 
      break;
  }
}

void RTFDocVisitor::visit(DocIncOperator *op)
{
  //printf("DocIncOperator: type=%d first=%d, last=%d text=`%s'\n",
  //    op->type(),op->isFirst(),op->isLast(),op->text().data());
  if (op->isFirst()) 
  {
    m_t << "\n\n\\footnotesize\\begin{verbatim}"; 
    m_hide = TRUE;
  }
  if (op->type()!=DocIncOperator::Skip) 
  {
    parseCode(m_ci,op->context(),op->text(),FALSE,0);
  }
  if (op->isLast())  
  {
    m_hide = FALSE;
    m_t << "\\end{verbatim}\\normalsize" << endl; 
  }
  else
  {
    m_t << endl;
  }
}

void RTFDocVisitor::visit(DocFormula *f)
{
  if (m_hide) return;
  m_t << f->text();
}

//--------------------------------------
// visitor functions for compound nodes
//--------------------------------------

void RTFDocVisitor::visitPre(DocAutoList *l)
{
  if (l->isEnumList())
  {
    m_t << "\\begin{enumerate}" << endl;
  }
  else
  {
    m_t << "\\begin{itemize}" << endl;
  }
}

void RTFDocVisitor::visitPost(DocAutoList *l)
{
  if (l->isEnumList())
  {
    m_t << "\\end{enumerate}" << endl;
  }
  else
  {
    m_t << "\\end{itemize}" << endl;
  }
}

void RTFDocVisitor::visitPre(DocAutoListItem *)
{
  m_t << "\\item ";
}

void RTFDocVisitor::visitPost(DocAutoListItem *) 
{
}

void RTFDocVisitor::visitPre(DocPara *) 
{
}

void RTFDocVisitor::visitPost(DocPara *p)
{
  if (!p->isLast() &&            // omit <p> for last paragraph
      !(p->parent() &&           // and for parameter sections
        p->parent()->kind()==DocNode::Kind_ParamSect
       )
     ) m_t << endl << endl;
}

void RTFDocVisitor::visitPre(DocRoot *)
{
}

void RTFDocVisitor::visitPost(DocRoot *)
{
}

void RTFDocVisitor::visitPre(DocSimpleSect *s)
{
  m_t << "\\begin{Desc}\n\\item[";
  switch(s->type())
  {
    case DocSimpleSect::See: 
      m_t << theTranslator->trSeeAlso(); break;
    case DocSimpleSect::Return: 
      m_t << theTranslator->trReturns(); break;
    case DocSimpleSect::Author: 
      m_t << theTranslator->trAuthor(TRUE,TRUE); break;
    case DocSimpleSect::Authors: 
      m_t << theTranslator->trAuthor(TRUE,FALSE); break;
    case DocSimpleSect::Version: 
      m_t << theTranslator->trVersion(); break;
    case DocSimpleSect::Since: 
      m_t << theTranslator->trSince(); break;
    case DocSimpleSect::Date: 
      m_t << theTranslator->trDate(); break;
    case DocSimpleSect::Note: 
      m_t << theTranslator->trNote(); break;
    case DocSimpleSect::Warning:
      m_t << theTranslator->trWarning(); break;
    case DocSimpleSect::Pre:
      m_t << theTranslator->trPrecondition(); break;
    case DocSimpleSect::Post:
      m_t << theTranslator->trPostcondition(); break;
    case DocSimpleSect::Invar:
      m_t << theTranslator->trInvariant(); break;
    case DocSimpleSect::Remark:
      m_t << theTranslator->trRemarks(); break;
    case DocSimpleSect::Attention:
      m_t << theTranslator->trAttention(); break;
    case DocSimpleSect::User: break;
    case DocSimpleSect::Unknown:  break;
  }

  // special case 1: user defined title
  if (s->type()!=DocSimpleSect::User)
  {
    m_t << ":]";
  }
}

void RTFDocVisitor::visitPost(DocSimpleSect *)
{
  m_t << "\\end{Desc}" << endl;
}

void RTFDocVisitor::visitPre(DocTitle *)
{
}

void RTFDocVisitor::visitPost(DocTitle *)
{
  m_t << "]";
}

void RTFDocVisitor::visitPre(DocSimpleList *)
{
  m_t << "\\begin{itemize}" << endl;
}

void RTFDocVisitor::visitPost(DocSimpleList *)
{
  m_t << "\\end{itemize}" << endl;
}

void RTFDocVisitor::visitPre(DocSimpleListItem *)
{
  m_t << "\\item ";
}

void RTFDocVisitor::visitPost(DocSimpleListItem *) 
{
}

void RTFDocVisitor::visitPre(DocSection *s)
{
  if (Config_getBool("PDF_HYPERLINKS"))
  {
    m_t << "\\hypertarget{" << s->file() << "_" << s->anchor() << "}{}";
  }
  if (s->level()==1)
  {
    if (Config_getBool("COMPACT_LATEX"))
    {
      m_t << "\\subsubsection{";
    }
    else
    {
      m_t << "\\subsection{";
    }
  }
  else if (s->level()==2)
  {
    if (Config_getBool("COMPACT_LATEX"))
    {
      m_t << "\\paragraph{";
    }
    else
    {
      m_t << "\\subsubsection{";
    }
  }
  filter(s->title());
  m_t << "}\\label{" << s->anchor() << "}" << endl;
}

void RTFDocVisitor::visitPost(DocSection *) 
{
}

void RTFDocVisitor::visitPre(DocHtmlList *s)
{
  if (s->type()==DocHtmlList::Ordered) 
    m_t << "\\begin{enumerate}" << endl; 
  else 
    m_t << "\\begin{itemize}" << endl;
}

void RTFDocVisitor::visitPost(DocHtmlList *s) 
{
  if (s->type()==DocHtmlList::Ordered) 
    m_t << "\\end{enumerate}" << endl; 
  else 
    m_t << "\\end{itemize}" << endl;
}

void RTFDocVisitor::visitPre(DocHtmlListItem *)
{
  m_t << "\\item ";
}

void RTFDocVisitor::visitPost(DocHtmlListItem *) 
{
}

void RTFDocVisitor::visitPre(DocHtmlPre *)
{
  m_t << "\\small\\begin{alltt}";
  m_insidePre=TRUE;
}

void RTFDocVisitor::visitPost(DocHtmlPre *) 
{
  m_insidePre=FALSE;
  m_t << "\\end{alltt}\\normalsize " << endl;
}

void RTFDocVisitor::visitPre(DocHtmlDescList *)
{
  m_t << "\\begin{description}" << endl;
}

void RTFDocVisitor::visitPost(DocHtmlDescList *) 
{
  m_t << "\\end{description}" << endl;
}

void RTFDocVisitor::visitPre(DocHtmlDescTitle *)
{
  m_t << "\\item[";
}

void RTFDocVisitor::visitPost(DocHtmlDescTitle *) 
{
  m_t << "]";
}

void RTFDocVisitor::visitPre(DocHtmlDescData *)
{
}

void RTFDocVisitor::visitPost(DocHtmlDescData *) 
{
}

void RTFDocVisitor::visitPre(DocHtmlTable *t)
{
  if (t->hasCaption()) 
  {
    m_t << "\\begin{table}[h]";
  }
  m_t << "\\begin{TabularC}{" << t->numCols() << "}\n\\hline\n";
}

void RTFDocVisitor::visitPost(DocHtmlTable *t) 
{
  if (t->hasCaption())
  {
    m_t << "\\end{table}\n";
  }
  else
  {
    m_t << "\\\\\\hline\n\\end{TabularC}\n";
  }
}

void RTFDocVisitor::visitPre(DocHtmlCaption *)
{
  m_t << "\\\\\\hline\n\\end{TabularC}\n\\centering\n\\caption{";
}

void RTFDocVisitor::visitPost(DocHtmlCaption *) 
{
  m_t << "}\n";
}

void RTFDocVisitor::visitPre(DocHtmlRow *)
{
}

void RTFDocVisitor::visitPost(DocHtmlRow *) 
{
  m_t << "\\\\\\hline\n";
}

void RTFDocVisitor::visitPre(DocHtmlCell *)
{
}

void RTFDocVisitor::visitPost(DocHtmlCell *c) 
{
  if (!c->isLast()) m_t << "&";
}

void RTFDocVisitor::visitPre(DocIndexEntry *)
{
  m_hide = TRUE;
}

void RTFDocVisitor::visitPost(DocIndexEntry *) 
{
  m_hide = FALSE;
}

void RTFDocVisitor::visitPre(DocInternal *)
{
  m_t << "\\begin{Desc}" << endl 
    << "\\item[" << theTranslator->trForInternalUseOnly() << "]" << endl;
}

void RTFDocVisitor::visitPost(DocInternal *) 
{
  m_t << "\\end{Desc}" << endl;
}

void RTFDocVisitor::visitPre(DocHRef *href)
{
  if (Config_getBool("PDF_HYPERLINKS"))
  {
    m_t << "\\href{";
    m_t << href->url();
    m_t << "}";
  }
  m_t << "{\\tt ";
}

void RTFDocVisitor::visitPost(DocHRef *) 
{
  m_t << "}";
}

void RTFDocVisitor::visitPre(DocHtmlHeader *header)
{
  if (Config_getBool("COMPACT_LATEX"))
  {
    switch(header->level())
    {
      case 1: m_t << "\\subsection{"; break;
      case 2: m_t << "\\subsubsection{"; break;
      case 3: m_t << "\\paragraph{"; break;
    }
  }
  else
  {
    switch(header->level())
    {
      case 1: m_t << "\\section{"; break;
      case 2: m_t << "\\subsection{"; break;
      case 3: m_t << "\\subsubsection{"; break;
    }
  }
}

void RTFDocVisitor::visitPost(DocHtmlHeader *) 
{
  m_t << "}";
}

void RTFDocVisitor::visitPre(DocImage *img)
{
  if (img->type()==DocImage::Latex)
  {
    if (img->hasCaption())
    {
      m_t << "\\begin{figure}[H]" << endl;
      m_t << "\\begin{center}" << endl;
    }
    else
    {
      m_t << "\\mbox{";
    }
    QCString gfxName = img->name();
    if (gfxName.right(4)==".eps" || gfxName.right(4)==".pdf")
    {
      gfxName=gfxName.left(gfxName.length()-4);
    }
    m_t << "\\includegraphics";
    if (!img->width().isEmpty())
    {
      m_t << "[width=" << img->width() << "]";
    }
    else if (!img->height().isEmpty())
    {
      m_t << "[height=" << img->height() << "]";
    }
    m_t << "{" << gfxName << "}";
    if (img->hasCaption())
    {
      m_t << "\\caption{";
    }
  }
  else // other format -> skip
  {
    m_hide=TRUE;
  }
}

void RTFDocVisitor::visitPost(DocImage *img) 
{
  if (img->type()==DocImage::Latex)
  {
    m_t << "}" << endl; // end mbox or caption
    if (img->hasCaption())
    {
      m_t << "\\end{center}" << endl;
      m_t << "\\end{figure}" << endl;
    }
  }
  else // other format
  {
    m_hide=FALSE;
  }
}

void RTFDocVisitor::visitPre(DocDotFile *df)
{
  QCString baseName=df->file();
  int i;
  if ((i=baseName.findRev('/'))!=-1)
  {
    baseName=baseName.right(baseName.length()-i-1);
  } 
  if (baseName.right(4)==".eps" || baseName.right(4)==".pdf")
  {
    baseName=baseName.left(baseName.length()-4);
  }
  QCString outDir = Config_getString("LATEX_OUTPUT");
  writeDotGraphFromFile(df->file(),outDir,baseName,EPS);
  if (df->hasCaption())
  {
    m_t << "\\begin{figure}[H]" << endl;
    m_t << "\\begin{center}" << endl;
  }
  else
  {
    m_t << "\\mbox{";
  }
  m_t << "\\includegraphics";
  if (!df->width().isEmpty())
  {
    m_t << "[width=" << df->width() << "]";
  }
  else if (!df->height().isEmpty())
  {
    m_t << "[height=" << df->height() << "]";
  }
  m_t << "{" << baseName << "}";

  if (df->hasCaption())
  {
    m_t << "\\caption{";
  }
}

void RTFDocVisitor::visitPost(DocDotFile *df) 
{
  m_t << "}" << endl; // end mbox or caption
  if (df->hasCaption())
  {
    m_t << "\\end{center}" << endl;
    m_t << "\\end{figure}" << endl;
  }
}

void RTFDocVisitor::visitPre(DocLink *lnk)
{
  startLink(lnk->ref(),lnk->file(),lnk->anchor());
}

void RTFDocVisitor::visitPost(DocLink *lnk) 
{
  endLink(lnk->ref());
}

void RTFDocVisitor::visitPre(DocRef *ref)
{
  startLink(ref->ref(),ref->file(),ref->anchor());
  if (!ref->hasLinkText()) filter(ref->targetTitle());
}

void RTFDocVisitor::visitPost(DocRef *ref) 
{
  endLink(ref->ref());
  m_t << " ";
}

void RTFDocVisitor::visitPre(DocSecRefItem *)
{
  m_t << "\\item \\contentsline{section}{";
}

void RTFDocVisitor::visitPost(DocSecRefItem *ref) 
{
  m_t << "}{\\ref{" << ref->anchor() << "}}{}" << endl;
}

void RTFDocVisitor::visitPre(DocSecRefList *)
{
  m_t << "\\footnotesize" << endl;
  m_t << "\\begin{multicols}{2}" << endl;
  m_t << "\\begin{CompactList}" << endl;
}

void RTFDocVisitor::visitPost(DocSecRefList *) 
{
  m_t << "\\end{CompactList}" << endl;
  m_t << "\\end{multicols}" << endl;
  m_t << "\\normalsize" << endl;
}

void RTFDocVisitor::visitPre(DocLanguage *)
{
}

void RTFDocVisitor::visitPost(DocLanguage *) 
{
}

void RTFDocVisitor::visitPre(DocParamSect *s)
{
  m_t << "\\begin{Desc}" << endl;
  m_t << "\\item[";
  switch(s->type())
  {
    case DocParamSect::Param: 
      m_t << theTranslator->trParameters(); break;
    case DocParamSect::RetVal: 
      m_t << theTranslator->trReturnValues(); break;
    case DocParamSect::Exception: 
      m_t << theTranslator->trExceptions(); break;
    default:
      ASSERT(0);
  }
  m_t << ":]" << endl;
  m_t << "\\begin{description}" << endl;
}

void RTFDocVisitor::visitPost(DocParamSect *)
{
  m_t << "\\end{description}" << endl;
  m_t << "\\end{Desc}" << endl;
}

void RTFDocVisitor::visitPre(DocParamList *pl)
{
  m_t << "\\item[{\\em ";
  QStrListIterator li(pl->parameters());
  const char *s;
  bool first=TRUE;
  for (li.toFirst();(s=li.current());++li)
  {
    if (!first) m_t << ","; else first=FALSE;
    m_t << s;
  }
  m_t << "}]";
}

void RTFDocVisitor::visitPost(DocParamList *)
{
}

void RTFDocVisitor::visitPre(DocXRefItem *x)
{
  m_t << "\\begin{Desc}" << endl;
  m_t << "\\item[";
  if (Config_getBool("PDF_HYPERLINKS"))
  {
    m_t << "\\hyperlink{" << x->file() << "_" << x->anchor() << "}{";
  }
  else
  {
    m_t << "{\\bf ";
  }
  filter(x->title());
  m_t << "}]";
}

void RTFDocVisitor::visitPost(DocXRefItem *)
{
  m_t << "\\end{Desc}" << endl;
}

void RTFDocVisitor::visitPre(DocInternalRef *ref)
{
  startLink("",ref->file(),ref->anchor());
}

void RTFDocVisitor::visitPost(DocInternalRef *) 
{
  endLink("");
  m_t << " ";
}

void RTFDocVisitor::visitPre(DocCopy *)
{
}

void RTFDocVisitor::visitPost(DocCopy *)
{
}

static char* getMultiByte(int c)
{
    static char s[10];
    sprintf(s,"\\'%X",c);
    return s;
}

void RTFDocVisitor::filter(const char *str)
{ 
  if (str)
  {
    const unsigned char *p=(const unsigned char *)str;
    unsigned char c;
    unsigned char pc='\0';
    while (*p)
    {
      static bool MultiByte = FALSE;
      c=*p++;

      if ( MultiByte )
      {
        m_t << getMultiByte( c );
        MultiByte = FALSE;
        continue;
      }
      if ( c >= 0x80 )
      {
        MultiByte = TRUE;
        m_t << getMultiByte( c );
        continue;
      }

      switch (c)
      {
        case '{':  m_t << "\\{";            break;
        case '}':  m_t << "\\}";            break;
        case '\\': m_t << "\\\\";           break;
        default:   m_t << (char)c;
      }
      pc = c;
    }
  }
}

void RTFDocVisitor::startLink(const QCString &ref,const QCString &file,const QCString &anchor)
{
  if (ref.isEmpty() && Config_getBool("RTF_HYPERLINKS"))
  {
    QCString refName;
    if (!file.isEmpty())
    {
      refName+=file;
    }
    if (anchor)
    {
      refName+='_';
      refName+=anchor;
    }

    m_t << "{\\field {\\*\\fldinst { HYPERLINK  \\\\l \"";
    m_t << rtfFormatBmkStr(refName);
    m_t << "\" }{}";
    m_t << "}{\\fldrslt {\\cs37\\ul\\cf2 ";
  }
  else
  {
    m_t << "{\\b ";
  }
}

void RTFDocVisitor::endLink(const QCString &ref)
{
  if (ref.isEmpty() && Config_getBool("RTF_HYPERLINKS"))
  {
    m_t << "}}}";
  }
  else
  {
    m_t << "}";
  }
}
