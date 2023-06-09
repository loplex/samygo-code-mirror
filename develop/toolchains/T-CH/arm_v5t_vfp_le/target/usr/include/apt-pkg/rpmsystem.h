// -*- mode: cpp; mode: fold -*-
// Description								/*{{{*/
// $Id: rpmsystem.h,v 1.2 2002/07/30 20:43:41 niemeyer Exp $
/* ######################################################################

   System - RPM version of the  System Class

   ##################################################################### 
 */
									/*}}}*/
#ifndef PKGLIB_RPMSYSTEM_H
#define PKGLIB_RPMSYSTEM_H

#ifdef __GNUG__
#pragma interface "apt-pkg/rpmsystem.h"
#endif

#include <apt-pkg/pkgsystem.h>
#include <apt-pkg/rpmindexfile.h>
#include <apt-pkg/algorithms.h>

#include <map>

class RPMDBHandler;
class pkgSourceList;
class rpmIndexFile;

class rpmSystem : public pkgSystem
{
   int LockCount;
   RPMDBHandler *RpmDB;
   rpmDatabaseIndex *StatusFile;
   
#ifdef OLD_FILEDEPS
   map<string,int> FileDeps; // file dependencies
#endif
#ifdef OLD_BESTARCH
   map<string,string> MultiArchPkgs; // packages with multi-archs
#endif

   bool processIndexFile(rpmIndexFile *Handler,OpProgress &Progress);
   
   public:

#ifdef OLD_FILEDEPS
   bool IsFileDep(string File);
#endif
#ifdef OLD_BESTARCH
   string BestArchForPackage(string Pkg);
   virtual bool PreProcess(pkgIndexFile **Start,pkgIndexFile **End,
			   OpProgress &Progress);
#endif
   RPMDBHandler *GetDBHandler();
   
   virtual bool LockRead();
   virtual bool Lock();
   virtual bool UnLock(bool NoErrors = false);
   virtual pkgPackageManager *CreatePM(pkgDepCache *Cache) const;
   virtual bool Initialize(Configuration &Cnf);
   virtual bool ArchiveSupported(const char *Type);
   virtual signed Score(Configuration const &Cnf);
   virtual string DistroVer();
   virtual bool AddStatusFiles(vector<pkgIndexFile *> &List);
   virtual bool AddSourceFiles(vector<pkgIndexFile *> &List);
   virtual bool FindIndex(pkgCache::PkgFileIterator File,
			  pkgIndexFile *&Found) const;
   virtual bool ProcessCache(pkgDepCache &Cache,pkgProblemResolver &Fix);
   virtual bool IgnoreDep(pkgVersioningSystem &VS,pkgCache::DepIterator &Dep);
   virtual void CacheBuilt();

   virtual unsigned long OptionsHash() const;

   rpmSystem();
   virtual ~rpmSystem();
};

extern rpmSystem rpmSys;
extern int HideZeroEpoch;

#endif
