/******************************************************************************
 ** Filename:    cutoffs.c
 ** Purpose:     Routines to manipulate an array of class cutoffs.
 ** Author:      Dan Johnson
 **
 ** (c) Copyright Hewlett-Packard Company, 1988.
 ** Licensed under the Apache License, Version 2.0 (the "License");
 ** you may not use this file except in compliance with the License.
 ** You may obtain a copy of the License at
 ** http://www.apache.org/licenses/LICENSE-2.0
 ** Unless required by applicable law or agreed to in writing, software
 ** distributed under the License is distributed on an "AS IS" BASIS,
 ** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 ** See the License for the specific language governing permissions and
 ** limitations under the License.
 ******************************************************************************/
/*----------------------------------------------------------------------------
          Include Files and Type Defines
----------------------------------------------------------------------------*/
#include "cutoffs.h"

#include <cstdio>

#include "classify.h"
#include "helpers.h"
#include "serialis.h"
#include "unichar.h"

#define REALLY_QUOTE_IT(x) QUOTE_IT(x)

#define MAX_CUTOFF      1000

namespace tesseract {
/**
 * Open file, read in all of the class-id/cutoff pairs
 * and insert them into the Cutoffs array.  Cutoffs are
 * indexed in the array by class id.  Unused entries in the
 * array are set to an arbitrarily high cutoff value.
 * @param fp file containing cutoff definitions
 * @param Cutoffs array to put cutoffs into
 */
void Classify::ReadNewCutoffs(TFile* fp, CLASS_CUTOFF_ARRAY Cutoffs) {
  char Class[UNICHAR_LEN + 1];
  CLASS_ID ClassId;
  int Cutoff;

  if (shape_table_ != nullptr) {
    if (!shapetable_cutoffs_.DeSerialize(fp)) {
      tprintf("Error during read of shapetable pffmtable!\n");
    }
  }
  for (int i = 0; i < MAX_NUM_CLASSES; i++)
    Cutoffs[i] = MAX_CUTOFF;

  const int kMaxLineSize = 100;
  char line[kMaxLineSize];
  while (fp->FGets(line, kMaxLineSize) != nullptr &&
         sscanf(line, "%" REALLY_QUOTE_IT(UNICHAR_LEN) "s %d", Class,
                &Cutoff) == 2) {
    if (strcmp(Class, "NULL") == 0) {
      ClassId = unicharset.unichar_to_id(" ");
    } else {
      ClassId = unicharset.unichar_to_id(Class);
    }
    Cutoffs[ClassId] = Cutoff;
  }
}

}  // namespace tesseract
