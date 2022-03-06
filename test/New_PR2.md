
Hey, thanks for responding so quickly!

> Let's finish merging the work by @elral:
>   [...]
>   Once we have done this we have a new baseline for further improvements.

That's exactly what I asked for in my request: to not let it float for too long, so we can be sure of the base we are working on.
Small and safe steps are only possible if the previous steps are already laid.

> If we don't finish these changes first we will have a lot of headache with merging changes back and forth into branches.

Fully agreed... and that is exactly my main concern. That's why I tried to prepare a set of files that could maybe bring to a quicker acceptance.

> A general thought and decision that i have taken is that i want to focus on a better and easier way allowing people to bring their own (exotic) devices to Mobiflight.
> This means that we have to be careful to not make the core too complicated to understand.

I don't really see this being currently an issue... at least, I don't see anything that might be making this aspect worse, but rather the opposite. 

> The general idea is to not include all these different devices into the official MobiFlight Core Firmware but focus on simple yet flexible interfaces for adding individual stuff if a user wants to. We can then allow users to build their own firmware if they need it. things like, I would like TMxxxx instead of Max7219 will then be possible.
> 
This is a good idea, and I think the firmware is remarkably modular in this respect, as firmware goes.  
Still, I think it is most applicable to really "exotic" or custom devices. There are still a few devices of current use - or used in certain configurations - that are eligible of being included in the core FW (e.g.: MCP23x17, the TM1637 you mention); but as things stand, this would imply having the possibility of conditional FW compilation, which - regardless of further device additions - is a topic that'll be worth considering.  
And another issue that's probably harder to tackle is that such a feature would involve the connector in a less trivial way, because I expect a need for more diverse interfaces on that side.

I would also mention that, for this goal, just a modicum of technical  documentation would probably be useful. Not end-user tutorials (which are also needed, and in larger quantity) but some basic information, like e.g. the documentation of the serial protocol or a brief overview of how the system works and how the software workflow is structured. 

> I am thinking of creating a place where we can collect community contributions, e.g. maintaining a list in a wiki/github.
> I strongly believe that decoupling the community devices from the MobiFlight Core Firmware will increase the speed with which we can support a broader range of ICs. Based on the community feedback and interest we can then still decide if a device will eventually make it into the MF Core Firmware. It's kind of like an "Incubator".

I'm not sure I understand what it is that you have in mind practically.  
To begin with, the firmware has undergone several optimizations and improvements regarding mainly the memory requirements (both RAM and Flash), for good reasons. Thinking of integrating contributions for external devices in the core is, in my opinion, a bit too stretched, since it requires some attention and knowledge not to break firmware operation... unless it all sums up to contributions as already done now, but then I don't quite see the point.  
On the other hand, allowing users to write their own firmware in order to support their own device types (or any other feature, for that matters), using a "base" version of the core firmware as a sort of sample / framework, that could be a very interesting provision.

> With this idea in mind, I believe the way how Ralf wanted to split and bundle the files was a good fit.

As I never held back, I think the splitting of device files goes against the goal of achieving more clarity and a better oversight when working with the source. But the main reason I asked Ralf if he would be inclined to dispense with it was because it would have complicated the introduction of the feature in subject. That said, it's something I can surely live with. 
 
> I will finish the current MF Connector Release and then I will be able to focus more on the firmware. It's just a matter of a couple of days until 9.3 will be released.

I would really appreciate it; most of what I'm working on is quickly grinding to a halt. For one, my PR for the multiplexers (which also involves the Connector) is dragging along and it's costing me a lot of effort for just keeping the pace of all new merges on the main branch.
