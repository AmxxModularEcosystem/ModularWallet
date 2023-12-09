stock SMW_Cvar(const sCvarName[],const sDefault[] = NULL_STRING) {
    new iCvar = create_cvar(sCvarName, sDefault);
    hook_cvar_change(iCvar, "@SMW_OnCvarChanged");
    @SMW_OnCvarChanged(iCvar);
}

@SMW_OnCvarChanged(const iCvar) {
    new sCurrencyName[MWALLET_CURRENCY_MAX_NAME_LEN];
    get_pcvar_string(iCvar, sCurrencyName, charsmax(sCurrencyName));
    
    SMW_SetCurrencyByName(sCurrencyName, false);
}
