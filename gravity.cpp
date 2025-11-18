#include <cmath>
#include <iostream>


class Vector {
private:
    double x_, y_;

public:
    Vector(double x = 0, double y = 0) : x_(x), y_(y) {}

    double getX() const { return x_; }
    double getY() const { return y_; }

    void set(double nx, double ny) { x_ = nx; y_ = ny; }

    Vector operator+(const Vector& other) const {
        return Vector(x_ + other.x_, y_ + other.y_);
    }
    Vector operator-(const Vector& other) const {
        return Vector(x_ - other.x_, y_ - other.y_);
    }
    Vector operator*(double s) const {
        return Vector(x_ * s, y_ * s);
    }
    Vector operator/(double s) const {
        return Vector(x_ / s, y_ / s);
    }

    double magnitude() const {
        return std::sqrt(x_ * x_ + y_ * y_);
    }

    Vector normalized() const {
        double m = magnitude();
        if (m == 0) return Vector(1, 0);
        return Vector(x_ / m, y_ / m);
    }
};


class Body {
protected:
    double mass_;
    Vector position_;
    Vector velocity_;

public:
    Body(double m, const Vector& p, const Vector& v)
            : mass_(m), position_(p), velocity_(v) {}

    virtual ~Body() {}

    // Getters
    double getMass() const { return mass_; }
    double getPosX() const { return position_.getX(); }
    double getPosY() const { return position_.getY(); }

    // Euler / Taylor update (PDF’deki Eş.(3)-(4))
    virtual void update(const Vector& totalForce, double dt) {
        Vector a = totalForce / mass_;

        velocity_ = velocity_ + a * dt;
        position_ = position_ + velocity_ * dt;
    }
};


class Rocket : public Body {
private:
    double exhaustVel_;
    double exhaustRate_;
    bool motorOn_;

public:
    Rocket(double m, const Vector& p, const Vector& v,
           double eVel, double eRate, bool motorOn = true)
            : Body(m, p, v),
              exhaustVel_(eVel), exhaustRate_(eRate),
              motorOn_(motorOn) {}

    void turnMotorOn() { motorOn_ = true; }
    void turnMotorOff() { motorOn_ = false; }

    bool isMotorOn() const { return motorOn_; }

    // Rocket ivme + thrust + kütle azalması
    virtual void update(const Vector& totalForce, double dt) override {
        Vector thrust(0, 0);

        if (motorOn_ && exhaustRate_ > 0 && mass_ > 0.1) {
            Vector dir = velocity_.normalized();
            thrust = dir * (exhaustVel_ * exhaustRate_);
            double newMass = mass_ - exhaustRate_ * dt;
            mass_ = (newMass < 0.1 ? 0.1 : newMass);
        }

        Vector netF = totalForce + thrust;
        Vector a = netF / mass_;

        velocity_ = velocity_ + a * dt;
        position_ = position_ + velocity_ * dt;
    }
};

class Gezegen : public Body {
public:
    Gezegen(double m, const Vector& p, const Vector& v)
            : Body(m, p, v) {}
};

class Asteroid : public Body {
public:
    Asteroid(double m, const Vector& p, const Vector& v)
            : Body(m, p, v) {}
};


struct BodyNode {
    Body* obj;
    BodyNode* next;

    BodyNode(Body* b) : obj(b), next(nullptr) {}
};

class BodyList {
private:
    BodyNode* head_;

public:
    BodyList() : head_(nullptr) {}

    ~BodyList() {
        BodyNode* c = head_;
        while (c) {
            BodyNode* n = c->next;
            delete c->obj;
            delete c;
            c = n;
        }
    }

    void append(Body* b) {
        BodyNode* node = new BodyNode(b);
        if (!head_) { head_ = node; return; }
        BodyNode* c = head_;
        while (c->next) c = c->next;
        c->next = node;
    }

    BodyNode* head() const { return head_; }

    int size() const {
        int count = 0;
        BodyNode* c = head_;
        while (c) { count++; c = c->next; }
        return count;
    }
};


class Universe {
private:
    double dt_;
    BodyList bodies_;

public:
    Universe(double dt) : dt_(dt) {}

    void addBody(Body* b) { bodies_.append(b); }

    // Zorunlu API – Örnek programla birebir uyumlu
    double getPosX(int idx) {
        BodyNode* c = bodies_.head();
        for (int i = 1; c && i < idx; i++) c = c->next;
        return c ? c->obj->getPosX() : 0;
    }

    double getPosY(int idx) {
        BodyNode* c = bodies_.head();
        for (int i = 1; c && i < idx; i++) c = c->next;
        return c ? c->obj->getPosY() : 0;
    }

    // Kütle çekimi hesapla + update et
    void run(double steps) {
        for (int s = 0; s < steps; s++) {
            int N = bodies_.size();
            if (N == 0) return;

            // 1) Tüm kuvvetleri önceden hesapla
            Vector* forces = new Vector[N];

            BodyNode* bi = bodies_.head();
            int i = 0;
            while (bi) {
                Vector totalF(0, 0);

                BodyNode* bj = bodies_.head();
                int j = 0;

                while (bj) {
                    if (i != j) {
                        Vector r = Vector(bj->obj->getPosX(), bj->obj->getPosY()) -
                                   Vector(bi->obj->getPosX(), bi->obj->getPosY());

                        double dist = r.magnitude();
                        if (dist < 0.01) dist = 0.01;

                        double F = (bi->obj->getMass() * bj->obj->getMass()) /
                                   (dist * dist * dist);

                        totalF = totalF + r * F;
                    }
                    bj = bj->next;
                    j++;
                }

                forces[i] = totalF;
                bi = bi->next;
                i++;
            }

            // 2) Update et
            bi = bodies_.head();
            i = 0;
            while (bi) {
                bi->obj->update(forces[i], dt_);
                bi = bi->next;
                i++;
            }

            delete[] forces;
        }
    }

    BodyList& getList() { return bodies_; }
};
