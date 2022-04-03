This is an update on the progress from the initial idea above.

#### Starting point:

Since the substantial PR #147 by @elral, I wanted to push his modifications a little bit further to completion.
Thanks to his work, most device objects are now no longer allocated statically, but placed in a common buffer, so (mostly) space is only assigned when required.
This implementation however has still some points that can be improved.

***TL;DR: Not all (parts of the) objects are allocated dynamically, plus some space is occupied by the index vectors (e.g. encoders[], ledSegments[]…)***

Technical details (you can skip this section if not interested):

- For __inputs__, device objects themselves are fully allocated in the common buffer, but index vectors with object pointers still exist.  
  _These vectors must be sized to contain each the maximum allowable number of each device type._
- For __outputs__, devices are made by the `MF device` object plus the actual "worker" object (e.g.: Steppers are made by one `MFStepper` + one `AccelStepper`; LED displays are made by an `MFsegments` plus a `LedControl` etc.).  
  The index vectors for output devices contain the whole `MF device` object (obviously larger than the pointers alone), while merely the "worker" object is placed in the common buffer.

The improvement consists in __removing index vectors altogether__ (using different object management structures) and placing __all__ data in the common buffer.  
__The elimination of index vectors allows to use just the exact RAM amount required, without the need for extra provisioning (either for pointers or whole objects).__   
__A related perk is that there is no longer a fixed limit for any kind of devices.__  
This solution allows a non negligible gain in RAM space (both because of higher efficiency and elimination of unnecessary provisioning), without any penalty in Flash occupation. (Gains have been quantified, but it’s a little hard to present them with a single global figure).

#### A sideline story:
In the meantime, following some comments regarding PR #152, I was trying to help by "de-composing" the individual changes included in the PR with the aim of putting them in the form of individual issues/PR pairs to speed up acceptance.

I started a branch with individual commits targeting following points:

1. Move files to new locations
2. `Mobiflight.cpp` split to `config.*` / `commandmessenger.*` / `inputHub.*`  / `outputHub.*` (see below)
3. Poll time schedules moved to `main`
4. `DEBUG2CMDMESSENGER` constant renamed to `DEBUG2MSG`
5. `MFEEPROM.init()` moved into the constructor
6. Removed obsolete Arduino includes for Arduino version < 1.00 (not included in #152)
7. Replaced "include guards" in .h files with `#pragma` directives

One thing I did not change yet was the splitting of client functions that use MFxxx device objects (e.g. `AddDevice()`) and input event handlers / output commands to individual files (e.g. `Encoder.*` + `MFEncoder.*`), because that would have been confusing for the sources I was working on. 
Instead, I gathered all these functions in two files (pairs), `InputHub.*` and `OutputHub.*`. These have become more manageable anyway because the functions contained have become smaller, so I saw no real need to split them anymore.

#### Putting it all together:

After some chats with @elral, during which I explained the whole situation to him, he kindly agreed that we could possibly proceed this way (pending his final approval):

- replace his PR #152 with a PR from my "detailed" branch;
- waive the file split for device client functions;
- on that base, add a new PR for my final modifications described above.
  
As soon as the final details are reviewed by @elral and me, I will immediately make the links to the repository branches available for evaluation.

#### Final request:

@DocMoebiuz @neilenns I would appreciate if you could briefly read the above and let me know if we can proceed this way or there are any suggestions or comments.
Thanks
