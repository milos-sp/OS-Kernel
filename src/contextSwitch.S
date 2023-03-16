.global _ZN3PCB13contextSwitchEPNS_7ContextES1_ //mozda ce meni biti pcb
.type _ZN3PCB13contextSwitchEPNS_7ContextES1_, @function

_ZN3PCB13contextSwitchEPNS_7ContextES1_:
    #a0 -> &oldContext, a1 -> &newContext
    sd ra, 0 * 8(a0) #ra je prvo polje u strukturi oldContext
    sd sp, 1 * 8(a0) #a sp je drugo, zato offset 8

    ld ra, 0 * 8(a1) #u a1 je drugi parametar, ostalo isto
    ld sp, 1 * 8(a1)

    ret