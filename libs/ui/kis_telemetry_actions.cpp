#include "kis_telemetry_actions.h"
#include "kis_telemetry_abstruct.h"

void KisToolsActivate::doAction(KisTelemetryAbstruct *provider,QString id)
{
   provider->doTicket(*this, id);
}

void KisToolsStartUse::doAction(KisTelemetryAbstruct *provider, QString id)
{
    provider->doTicket(*this, id);
}

void KisToolsStopUse::doAction(KisTelemetryAbstruct *provider, QString id)
{
    provider->doTicket(*this, id);

}

void KisToolsDeactivate::doAction(KisTelemetryAbstruct *provider, QString id)
{
    provider->doTicket(*this, id);
}