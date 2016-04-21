#include "Particles.h"
#include "World.h"
#include "Textures.h"

vector<Particles::Particle> Particles::ptcs;
double Particles::pxpos, Particles::pypos, Particles::pzpos;

void Particles::updateall()
{
    for (vector<Particle>::iterator iter = ptcs.begin(); iter < ptcs.end();)
        if (iter->exist)
        {
            iter->Update();
            if (iter->lasts <= 0)
            {
                iter->exist = false;
                iter = ptcs.erase(iter);
            }
            else
                iter++;
        }
}

void Particles::renderall(double xpos, double ypos, double zpos)
{
    pxpos = xpos;
    pypos = ypos;
    pzpos = zpos;
    for (Particle it:ptcs)
        if (it.exist)
            it.Render();
}

void Particles::throwParticle(block pt, float x, float y, float z, float xs, float ys, float zs, float psz, int last)
{
    float tcX1 = (float)Textures::getTexcoordX(pt, 2);
    float tcY1 = (float)Textures::getTexcoordY(pt, 2);
    Particle ptc;
    ptc.exist = true;
    ptc.xpos = x;
    ptc.ypos = y;
    ptc.zpos = z;
    ptc.xsp = xs;
    ptc.ysp = ys;
    ptc.zsp = zs;
    ptc.psize = psz;
    ptc.hb = Hitbox::AABB(x - psz, y - psz, z - psz, x + psz, y + psz, z + psz);
    ptc.lasts = last;
    ptc.tcX = tcX1 + (float)mersenne->get_double_co()*((float)BLOCKTEXTURE_UNITSIZE / BLOCKTEXTURE_SIZE)*(1.0f - psz);
    ptc.tcY = tcY1 + (float)mersenne->get_double_co()*((float)BLOCKTEXTURE_UNITSIZE / BLOCKTEXTURE_SIZE)*(1.0f - psz);
    ptcs.push_back(ptc);
}

void Particles::Particle::Update()
{
    double dx, dy, dz;
    float psz = psize;

    hb = Hitbox::AABB(xpos - psz, ypos - psz, zpos - psz, xpos + psz, ypos + psz, zpos + psz);
    dx = xsp;
    dy = ysp;
    dz = zsp;

    vector<Hitbox::AABB> Hitboxes = World::getHitboxes(Hitbox::Expand(hb, dx, dy, dz));
    for (size_t i = 0; i < Hitboxes.size(); i++)
    {
        dy = Hitbox::MaxMoveOnYclip(hb, Hitboxes[i], dy);
    }
    Hitbox::Move(hb, 0.0, dy, 0.0);
    for (size_t i = 0; i < Hitboxes.size(); i++)
    {
        dx = Hitbox::MaxMoveOnXclip(hb, Hitboxes[i], dx);
    }
    Hitbox::Move(hb, dx, 0.0, 0.0);
    for (size_t i = 0; i < Hitboxes.size(); i++)
    {
        dz = Hitbox::MaxMoveOnZclip(hb, Hitboxes[i], dz);
    }
    Hitbox::Move(hb, 0.0, 0.0, dz);

    xpos += dx;
    ypos += dy;
    zpos += dz;
    if (dy != ysp) ysp = 0.0;
    xsp *= 0.6f;
    zsp *= 0.6f;
    ysp -= 0.01f;
    lasts -= 1;
}

void Particles::Particle::Render()
{
    //if (!Frustum::aabbInFrustum(hb)) return;
    float size = (float)BLOCKTEXTURE_UNITSIZE / BLOCKTEXTURE_SIZE * (psize <= 1.0f ? psize : 1.0f);
    float col = World::getbrightness(RoundInt(xpos), RoundInt(ypos), RoundInt(zpos)) / (float)World::BRIGHTNESSMAX;
    float col1 = col * 0.5f;
    float col2 = col * 0.7f;
    double palpha = (lasts < 30 ? lasts / 30.0 : 1.0);
    double xpos = this->xpos - pxpos;
    double ypos = this->ypos - pypos;
    double zpos = this->zpos - pzpos;

    glBegin(GL_QUADS);
    glColor4d(col1, col1, col1, palpha);
    glTexCoord2d(tcX + size * 0.0, tcY + size * 0.0);
    glVertex3d(xpos - psize, ypos - psize, zpos + psize);
    glTexCoord2d(tcX + size * 1.0, tcY + size * 0.0);
    glVertex3d(xpos + psize, ypos - psize, zpos + psize);
    glTexCoord2d(tcX + size * 1.0, tcY + size * 1.0);
    glVertex3d(xpos + psize, ypos + psize, zpos + psize);
    glTexCoord2d(tcX + size * 0.0, tcY + size * 1.0);
    glVertex3d(xpos - psize, ypos + psize, zpos + psize);

    glColor4d(col1, col1, col1, palpha);
    glTexCoord2d(tcX + size * 0.0, tcY + size * 0.0);
    glVertex3d(xpos - psize, ypos + psize, zpos - psize);
    glTexCoord2d(tcX + size * 1.0, tcY + size * 0.0);
    glVertex3d(xpos + psize, ypos + psize, zpos - psize);
    glTexCoord2d(tcX + size * 1.0, tcY + size * 1.0);
    glVertex3d(xpos + psize, ypos - psize, zpos - psize);
    glTexCoord2d(tcX + size * 0.0, tcY + size * 1.0);
    glVertex3d(xpos - psize, ypos - psize, zpos - psize);

    glColor4d(col, col, col, palpha);
    glTexCoord2d(tcX + size * 0.0, tcY + size * 0.0);
    glVertex3d(xpos + psize, ypos + psize, zpos - psize);
    glTexCoord2d(tcX + size * 1.0, tcY + size * 0.0);
    glVertex3d(xpos - psize, ypos + psize, zpos - psize);
    glTexCoord2d(tcX + size * 1.0, tcY + size * 1.0);
    glVertex3d(xpos - psize, ypos + psize, zpos + psize);
    glTexCoord2d(tcX + size * 0.0, tcY + size * 1.0);
    glVertex3d(xpos + psize, ypos + psize, zpos + psize);

    glColor4d(col, col, col, palpha);
    glTexCoord2d(tcX + size * 0.0, tcY + size * 0.0);
    glVertex3d(xpos - psize, ypos - psize, zpos - psize);
    glTexCoord2d(tcX + size * 1.0, tcY + size * 0.0);
    glVertex3d(xpos + psize, ypos - psize, zpos - psize);
    glTexCoord2d(tcX + size * 1.0, tcY + size * 1.0);
    glVertex3d(xpos + psize, ypos - psize, zpos + psize);
    glTexCoord2d(tcX + size * 0.0, tcY + size * 1.0);
    glVertex3d(xpos - psize, ypos - psize, zpos + psize);

    glColor4d(col2, col2, col2, palpha);
    glTexCoord2d(tcX + size * 0.0, tcY + size * 0.0);
    glVertex3d(xpos + psize, ypos + psize, zpos - psize);
    glTexCoord2d(tcX + size * 1.0, tcY + size * 0.0);
    glVertex3d(xpos + psize, ypos + psize, zpos + psize);
    glTexCoord2d(tcX + size * 1.0, tcY + size * 1.0);
    glVertex3d(xpos + psize, ypos - psize, zpos + psize);
    glTexCoord2d(tcX + size * 0.0, tcY + size * 1.0);
    glVertex3d(xpos + psize, ypos - psize, zpos - psize);

    glColor4d(col2, col2, col2, palpha);
    glTexCoord2d(tcX + size * 0.0, tcY + size * 0.0);
    glVertex3d(xpos - psize, ypos - psize, zpos - psize);
    glTexCoord2d(tcX + size * 1.0, tcY + size * 0.0);
    glVertex3d(xpos - psize, ypos - psize, zpos + psize);
    glTexCoord2d(tcX + size * 1.0, tcY + size * 1.0);
    glVertex3d(xpos - psize, ypos + psize, zpos + psize);
    glTexCoord2d(tcX + size * 0.0, tcY + size * 1.0);
    glVertex3d(xpos - psize, ypos + psize, zpos - psize);
    glEnd();
}
