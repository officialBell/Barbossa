#include "main.h"

void SinCos(float radians, float *sine, float *cosine)
{
    double __cosr, __sinr;
    __asm ("fsincos" : "=t" (__cosr), "=u" (__sinr) : "0" (radians));
    
    *sine = __sinr;
    *cosine = __cosr;
}

void AngleVectors(const Vector & angles, Vector * forward) {
    Assert(s_bMathlibInitialized);
    Assert(forward);
    
    float	sp, sy, cp, cy;
    
    sy = sin(DEG2RAD(angles[1]));
    cy = cos(DEG2RAD(angles[1]));
    
    sp = sin(DEG2RAD(angles[0]));
    cp = cos(DEG2RAD(angles[0]));
    
    forward->x = cp*cy;
    forward->y = cp*sy;
    forward->z = -sp;
}

void AngleVectors(const Vector & angles, Vector * forward, Vector * right, Vector * up) {
    float angle;
    static float sr, sp, sy, cr, cp, cy;
    
    angle = angles[1] * (M_PI * 2 / 360);
    sy = sin(angle);
    cy = cos(angle);
    
    angle = angles[0] * (M_PI * 2 / 360);
    sp = sin(angle);
    cp = cos(angle);
    
    angle = angles[2] * (M_PI * 2 / 360);
    sr = sin(angle);
    cr = cos(angle);
    
    if (forward)
    {
        forward->x = cp*cy;
        forward->y = cp*sy;
        forward->z = -sp;
    }
    if (right)
    {
        right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
        right->y = (-1 * sr*sp*sy + -1 * cr*cy);
        right->z = -1 * sr*cp;
    }
    if (up)
    {
        up->x = (cr*sp*cy + -sr*-sy);
        up->y = (cr*sp*sy + -sr*cy);
        up->z = cr*cp;
    }
}

void VectorAngles(const Vector& forward, Vector &angles) {
    float	tmp, yaw, pitch;
    
    if (forward[1] == 0 && forward[0] == 0)
    {
        yaw = 0;
        if (forward[2] > 0)
            pitch = 270;
        else
            pitch = 90;
    }
    else
    {
        yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
        if (yaw < 0)
            yaw += 360;
        
        tmp = sqrt (forward[0]*forward[0] + forward[1]*forward[1]);
        pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
        if (pitch < 0)
            pitch += 360;
    }
    
    angles[0] = pitch;
    angles[1] = yaw;
    angles[2] = 0;
}


void CorrectMovement(Vector vOldAngles, CUserCmd* pCmd, float fOldForward, float fOldSidemove)
{
    // side/forward move correction
    float deltaView;
    float f1;
    float f2;
    
    if (vOldAngles.y < 0.f)
        f1 = 360.0f + vOldAngles.y;
    else
        f1 = vOldAngles.y;
    
    if (pCmd->viewangles.y < 0.0f)
        f2 = 360.0f + pCmd->viewangles.y;
    else
        f2 = pCmd->viewangles.y;
    
    if (f2 < f1)
        deltaView = abs(f2 - f1);
    else
        deltaView = 360.0f - abs(f1 - f2);
    
    deltaView = 360.0f - deltaView;
    
    pCmd->forwardmove = cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
    pCmd->sidemove = sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
}

bool WorldToScreen(Vector& vFrom, Vector& vTo) {
    return (pOverlay->ScreenPosition(vFrom, vTo) != 1);
}



bool DrawPlayerBox(C_BaseEntity* pEntity, boxstruct& str) // Credit's to dude719 & keybode.
{
    if (!pEntity)
        return false;
    
    Vector vOrigin = pEntity->GetVecOrigin(); // Not sure if this is the right one...
    Vector min = pEntity->GetCollideable()->OBBMins() + vOrigin;
    Vector max = pEntity->GetCollideable()->OBBMaxs() + vOrigin;
    
    Vector points[] = { Vector(min.x, min.y, min.z),
        Vector(min.x, max.y, min.z),
        Vector(max.x, max.y, min.z),
        Vector(max.x, min.y, min.z),
        Vector(max.x, max.y, max.z),
        Vector(min.x, max.y, max.z),
        Vector(min.x, min.y, max.z),
        Vector(max.x, min.y, max.z) };
    
    /*
     .6-------5                            blb = 0
     .' |       .' |                            brb = 1
     7---+--4'   |        0 = min          frb = 2
     |   |    |    |        4 = max         flb = 3
     y |  ,0---+---1                            frt = 4
     |.'       | .'  z                           brt = 5
     3------2                                  blt = 6
     x                                        flt = 7
     
     blt------brt        blt = back-left-top
     .' |        .'|            brt = back-right-top
     flt---+--frt  |            brb = back-right-bottom
     |   |     |   |            blb = back-left-bottom
     y |  blb---+--brb        flt = front-left-top
     |.'        | .'  z        frt = front-right-top
     flb------frb            frb = front-right-bottom
     x                    flb = front-left-bottom
     
     */
    
    Vector flb;
    Vector brt;
    Vector blb;
    Vector frt;
    Vector frb;
    Vector brb;
    Vector blt;
    Vector flt;
    
    
    if (!WorldToScreen(points[3], flb) || !WorldToScreen(points[5], brt)
        || !WorldToScreen(points[0], blb) || !WorldToScreen(points[4], frt)
        || !WorldToScreen(points[2], frb) || !WorldToScreen(points[1], brb)
        || !WorldToScreen(points[6], blt) || !WorldToScreen(points[7], flt))
        return false;
    
    Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };
    
    float left = flb.x;        // left
    float top = flb.y;        // top
    float right = flb.x;    // right
    float bottom = flb.y;    // bottom
    
    for (int i = 1; i < 8; i++)
    {
        if (left > arr[i].x)
            left = arr[i].x;
        if (bottom < arr[i].y)
            bottom = arr[i].y;
        if (right < arr[i].x)
            right = arr[i].x;
        if (top > arr[i].y)
            top = arr[i].y;
    }
    
    /*
     
     
              ^
              |
             TOP
     
     <-- LEFT   RIGHT -->
     
           BOTTOM
             |
             v
     
     
     
     */
    
    str.x = left;
    str.y = top;
    str.w = right - left;
    str.h = bottom - top;
    return true;
}

void Normalize(Vector & vIn, Vector & vOut) {
    float flLen = vIn.Length();
    if (flLen == 0) {
        vOut.Init(0, 0, 1);
        return;
    }
    flLen = 1 / flLen;
    vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
}


void GetEpochTime()
{
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

void StdReplaceStr(std::string& replaceIn, const std::string& replace, const std::string& replacewith)
{
    size_t const span = replace.size();
    size_t const step = replacewith.size();
    size_t index = 0;
    
    while (true)
    {
        index = replaceIn.find(replace, index);
        
        if(index == std::string::npos)
            break;
        
        replaceIn.replace(index, span, replacewith);
        index += step;
    }
}

const char* PadStringRight(std::string text, size_t value)
{
    text.insert(text.length(), value - text.length(), ' ');
    
    return text.c_str();
}

void VectorTransform(Vector& in1, matrix3x4_t& in2, Vector& out) {
    out.x = DotProduct(in1, Vector(in2[0][0], in2[0][1], in2[0][2])) + in2[0][3];
    out.y = DotProduct(in1, Vector(in2[1][0], in2[1][1], in2[1][2])) + in2[1][3];
    out.z = DotProduct(in1, Vector(in2[2][0], in2[2][1], in2[2][2])) + in2[2][3];
}



Vector GetHitboxPosition(C_BaseEntity* pEntity, int Hitbox) {
    matrix3x4_t matrix[128];
    
    if (!pEntity->SetupBones(matrix, 128, 0x100, pGlobals->curtime))
        return Vector(0, 0, 0);
    
    studiohdr_t* hdr = pModelInfo->GetStudioModel(pEntity->GetModel());
    mstudiohitboxset_t* set = hdr->pHitboxSet(0);
    
    mstudiobbox_t* hitbox = set->pHitbox(Hitbox);
    
    if (!hitbox)
        return Vector(0, 0, 0);
    
    Vector vMin, vMax, vCenter, sCenter;
    VectorTransform(hitbox->bbmin, matrix[hitbox->bone], vMin);
    VectorTransform(hitbox->bbmax, matrix[hitbox->bone], vMax);
    vCenter = (vMin + vMax) *0.5f;
    
    if(vars.aimbot.pointscale != 101) {
        float ptwoz = 50 - vars.aimbot.pointscale;
        float zval = (ptwoz*vMin.z + vars.aimbot.pointscale*vMax.z) / 50;
        vCenter.z = zval;
    }
    return vCenter;
}

bool IsVisible(C_BaseEntity* local, C_BaseEntity* entity) {
    Ray_t ray;
    trace_t trace;
    CTraceFilter filter;
    filter.pSkip = local;
    
    ray.Init(local->GetEyePosition(), entity->GetEyePosition());
    pEngineTrace->TraceRay(ray, 0x4600400B, &filter, &trace);
    return (trace.m_pEnt == entity || trace.fraction > 0.99f);
}

void FixMovement(Vector& oang, CUserCmd* pCmd) {
    Vector vMove(pCmd->forwardmove, pCmd->sidemove, pCmd->upmove);
    float flSpeed = sqrt(vMove.x * vMove.x + vMove.y * vMove.y), flYaw;
    Vector vMove2;
    VectorAngles(vMove, vMove2);
    
    flYaw = DEG2RAD(pCmd->viewangles.y - oang.y + vMove2.y);
    pCmd->forwardmove = cos(flYaw) * flSpeed;
    pCmd->sidemove = sin(flYaw) * flSpeed;
    
    if (pCmd->viewangles.x < -90.f || pCmd->viewangles.x > 90.f)
        pCmd->forwardmove = -pCmd->forwardmove;
}

void SetClanTag(const char* tag, const char* name)
{
    SetTag(tag, name);
}

void MoveFix(CUserCmd *cmd, Vector &realvec)
{
    Vector vMove(cmd->forwardmove, cmd->sidemove, cmd->upmove);
    float flSpeed = sqrt(vMove.x * vMove.x + vMove.y * vMove.y), flYaw;
    Vector vMove2;
    VectorAngles(vMove, vMove2);
    
    flYaw = DEG2RAD(cmd->viewangles.y - realvec.y + vMove2.y);
    cmd->forwardmove = cos(flYaw) * flSpeed;
    cmd->sidemove = sin(flYaw) * flSpeed;
    
    if (cmd->viewangles.x < -90.f || cmd->viewangles.x > 90.f)
        cmd->forwardmove = -cmd->forwardmove;
}

Vector CalcAngle(Vector src, Vector dst)
{
    Vector delta, angles;
    delta.x = (src.x - dst.x);
    delta.y = (src.y - dst.y);
    delta.z = (src.z - dst.z);
    
    double hyp = sqrt(delta.x * delta.x + delta.y * delta.y);
    
    angles.x = (float)(atanf(delta.z / hyp) * M_RADPI);
    angles.y = (float)(atanf(delta.y / delta.x) * M_RADPI);
    
    angles.z = 0.0f;
    if (delta.x >= 0.0)
    {
        angles.y += 180.0f;
    }
    return angles;
}

C_BaseCombatWeapon* GetActiveWeapon(C_BaseEntity* local)
{
    auto hWeapon = local->GetActiveWeapon();
    
    if (!hWeapon)
        return nullptr;
    
    return (C_BaseCombatWeapon*)pEntList->GetClientEntityFromHandle(hWeapon);
}

float GetFOV(Vector start, Vector src, Vector dst)
{
    Vector ang, aim;
    ang = CalcAngle(src, dst);
    AngleVectors(start, &aim);
    AngleVectors(ang, &ang);
    
    float mag_s = std::sqrt((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));
    float mag_d = mag_s;
    float u_dot_v = aim[0] * ang[0] + aim[1] * ang[1] + aim[2] * ang[2];
    float fov = std::acos(u_dot_v / (mag_s * mag_d)) * (180.f / M_PI_F);
    return fov;
}

Vector ExtrapolateTick( Vector p0, Vector v0 )
{
    return p0 + ( v0 * pGlobals->interval_per_tick );
}

Vector C_BaseEntity::GetPredicted( Vector p0 )
{
    return ExtrapolateTick( p0, this->GetVelocity() );
}

void ClampMoves(float& flForward, float& flSide, float& flUp)
{
    if(flForward > 450) flForward = 450;
    if(flSide > 450) flSide = 450;
    if(flUp > 450) flUp = 450;
    if(flForward < -450) flForward = -450;
    if(flSide < -450) flSide = -450;
    if(flUp < -450) flUp = -450;
}

void ClampYaw(float& angle)
{
    while(angle > 180)
    {
        angle -= 360;
    }
    while(angle < -180)
    {
        angle += 360;
    }
}

