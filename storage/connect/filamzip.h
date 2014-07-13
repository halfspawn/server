/************** FilAmZip H Declares Source Code File (.H) **************/
/*  Name: FILAMZIP.H    Version 1.1                                    */
/*                                                                     */
/*  (C) Copyright to the author Olivier BERTRAND          2005-2012    */
/*                                                                     */
/*  This file contains the GZIP access method classes declares.        */
/***********************************************************************/
#ifndef __FILAMZIP_H
#define __FILAMZIP_H

#include "zlib.h"

typedef class ZIPFAM *PZIPFAM;
typedef class ZBKFAM *PZBKFAM;
typedef class ZIXFAM *PZIXFAM;
typedef class ZLBFAM *PZLBFAM;

/***********************************************************************/
/*  This is the access method class declaration for not optimized      */
/*  variable record length files compressed using the gzip library     */
/*  functions. File is accessed record by record (row).                */
/***********************************************************************/
class DllExport ZIPFAM : public TXTFAM {
//  friend class DOSCOL;
 public:
  // Constructor
  ZIPFAM(PDOSDEF tdp) : TXTFAM(tdp) {Zfile = NULL; Zpos = 0;}
  ZIPFAM(PZIPFAM txfp);

  // Implementation
  virtual AMT  GetAmType(void) {return TYPE_AM_ZIP;}
  virtual int  GetPos(void);
  virtual int  GetNextPos(void);
  virtual PTXF Duplicate(PGLOBAL g)
                {return (PTXF)new(g) ZIPFAM(this);}

  // Methods
  virtual void Reset(void);
  virtual int  GetFileLength(PGLOBAL g);
  virtual int  Cardinality(PGLOBAL g) {return (g) ? -1 : 0;}
  virtual bool AllocateBuffer(PGLOBAL g);
  virtual int  GetRowID(void);
  virtual bool RecordPos(PGLOBAL g);
  virtual bool SetPos(PGLOBAL g, int recpos);
  virtual int  SkipRecord(PGLOBAL g, bool header);
  virtual bool OpenTableFile(PGLOBAL g);
  virtual int  ReadBuffer(PGLOBAL g);
  virtual int  WriteBuffer(PGLOBAL g);
  virtual int  DeleteRecords(PGLOBAL g, int irc);
  virtual void CloseTableFile(PGLOBAL g);
  virtual void Rewind(void);

 protected:
          int  Zerror(PGLOBAL g);    // GZ error function

  // Members
  gzFile  Zfile;              // Points to GZ file structure
  z_off_t Zpos;               // Uncompressed file position
  }; // end of class ZIPFAM

/***********************************************************************/
/*  This is the access method class declaration for optimized variable */
/*  record length files compressed using the gzip library functions.   */
/*  The File is accessed by block (requires an opt file).              */
/***********************************************************************/
class DllExport ZBKFAM : public ZIPFAM {
 public:
  // Constructor
  ZBKFAM(PDOSDEF tdp);
  ZBKFAM(PZBKFAM txfp);

  // Implementation
  virtual int  GetPos(void);
  virtual int  GetNextPos(void) {return 0;}
  virtual PTXF Duplicate(PGLOBAL g)
                {return (PTXF)new(g) ZBKFAM(this);}

  // Methods
  virtual int  Cardinality(PGLOBAL g);
  virtual bool AllocateBuffer(PGLOBAL g);
  virtual int  GetRowID(void);
  virtual bool RecordPos(PGLOBAL g);
  virtual int  SkipRecord(PGLOBAL g, bool header);
  virtual int  ReadBuffer(PGLOBAL g);
  virtual int  WriteBuffer(PGLOBAL g);
  virtual int  DeleteRecords(PGLOBAL g, int irc);
  virtual void CloseTableFile(PGLOBAL g);
  virtual void Rewind(void);

 protected:
  // Members
  char *CurLine;              // Position of current line in buffer
  char *NxtLine;              // Position of Next    line in buffer
  bool  Closing;              // True when closing on Insert
  }; // end of class ZBKFAM

/***********************************************************************/
/*  This is the access method class declaration for fixed record       */
/*  length files compressed using the gzip library functions.          */
/*  The file is always accessed by block.                              */
/***********************************************************************/
class DllExport ZIXFAM : public ZBKFAM {
 public:
  // Constructor
  ZIXFAM(PDOSDEF tdp);
  ZIXFAM(PZIXFAM txfp) : ZBKFAM(txfp) {}

  // Implementation
  virtual int  GetNextPos(void) {return 0;}
  virtual PTXF Duplicate(PGLOBAL g)
                {return (PTXF)new(g) ZIXFAM(this);}

  // Methods
  virtual int  Cardinality(PGLOBAL g);
  virtual bool AllocateBuffer(PGLOBAL g);
  virtual int  ReadBuffer(PGLOBAL g);
  virtual int  WriteBuffer(PGLOBAL g);

 protected:
  // No additional Members
  }; // end of class ZIXFAM

#if 0
/***********************************************************************/
/*  This is the DOS/UNIX Access Method class declaration for PlugDB    */
/*  fixed/variable files compressed using the zlib library functions.  */
/*  Physically these are written and read using the same technique     */
/*  than blocked variable files, only the contain of each block is     */
/*  compressed using the deflate zlib function. The purpose of this    */
/*  specific format is to have a fast mechanism for direct access of   */
/*  records so blocked optimization is fast and direct access (joins)  */
/*  is allowed. Note that the block length is written ahead of each    */
/*  block to enable reading when optimization file is not available.   */
/***********************************************************************/
class DllExport ZLBFAM : public BLKFAM {
 public:
  // Constructor
  ZLBFAM(PDOSDEF tdp);
  ZLBFAM(PZLBFAM txfp);

  // Implementation
  virtual AMT  GetAmType(void) {return TYPE_AM_ZLIB;}
  virtual int  GetPos(void);
  virtual int  GetNextPos(void);
  virtual PTXF Duplicate(PGLOBAL g)
                  {return (PTXF)new(g) ZLBFAM(this);}
  inline  void SetOptimized(bool b) {Optimized = b;}

  // Methods
  virtual int  GetFileLength(PGLOBAL g);
  virtual bool AllocateBuffer(PGLOBAL g);
  virtual int  ReadBuffer(PGLOBAL g);
  virtual int  WriteBuffer(PGLOBAL g);
  virtual void CloseTableFile(PGLOBAL g);
  virtual void Rewind(void);

 protected:
          bool WriteCompressedBuffer(PGLOBAL g);
          int  ReadCompressedBuffer(PGLOBAL g, void *rdbuf);

  // Members
  z_streamp Zstream;          // Compression/decompression stream
  Byte     *Zbuffer;          // Compressed block buffer
  int      *Zlenp;            // Pointer to block length
  bool      Optimized;        // true when opt file is available
  }; // end of class ZLBFAM
#endif // 0

#endif // __FILAMZIP_H