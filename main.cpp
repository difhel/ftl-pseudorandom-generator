#include <debug.h>
#include <assert.h>
#include <iostream>
#include <algorithm>
#include <iomanip>

using ll = int_fast64_t;
using ld = long double;

class Generator {
    public:
        struct Seed {
            Seed() : s1(0), s2(0), s3(0) {}
            Seed(ll s1, ll s2, ll s3) : s1(s1), s2(s2), s3(s3) {}
            ll s1;
            ll s2;
            ll s3;
            bool operator==(const Seed& other) const {
                return this->s1 == other.s1 &&
                       this->s2 == other.s2 &&
                       this->s3 == other.s3;
            }

            bool operator!=(const Seed& other) const {
                return !(*this == other);
            }
        };

        struct Coefficients {
            Coefficients() : a(0), b(0), c(0) {}
            Coefficients(ll a, ll b, ll c, ll d) : a(a), b(b), c(c), d(d) {}
            ll a;
            ll b;
            ll c;
            ll d;
        };

        Seed seed;

        bool operator==(const Generator& other) const {
            return this->seed == other.seed;
        }

        bool operator!=(const Generator& other) const {
            return !(*this == other);
        }

        Generator() : cfs_(Coefficients()), seed(Seed()) {}

        Generator(Coefficients coefficients, Seed seed) :
            cfs_(coefficients), seed(seed) {}

        ll generate() {
            ll result = mod(
                cfs_.a * seed.s3 + cfs_.b * seed.s2 + cfs_.c * seed.s1, cfs_.d);
            seed = Seed(seed.s2, seed.s3, result);
            return result;
        }

        Generator::Seed FindCollision() const {
            /* find the first collision using Achilles and turtle algorithm and return 
            the collision seed
            */
            Generator g_achilles = *this;
            Generator g_turtle = *this;
            ll achiless_v = (g_achilles.generate(), g_achilles.generate());
            ll turtle_v = g_turtle.generate();
            while (g_achilles != g_turtle) {
                achiless_v = (g_achilles.generate(), g_achilles.generate());
                turtle_v = g_turtle.generate();
            }

            #ifdef DEBUG
            std::cout << "Collision" << " ";
            std::cout << achiless_v << " " << turtle_v << "\n";
            std::cout << g_achilles.seed.s1 << " " << g_turtle.seed.s1 << "\n";
            #endif

            return g_achilles.seed;
        }

        ll CalcPeriod() const {
            // calc the period of the generator and returns
            // a pair of the period and the generator with
            // <period value> steps generated
            const Generator::Seed collision = FindCollision();
            ll period = 1;
            Generator period_generator = *this;
            while ((
                period_generator.generate(),
                period_generator.seed) != collision) {
                ++period;
            }

            #ifdef DEBUG
            std::cout << collision.s1 << " ";
            std::cout << collision.s2_ << " ";
            std::cout << collision.s3 << "\n";
            std::cout << "Period: " << period << "\n";
            #endif

            return period;
        }

        ll CalcHalfPeriod(ll period) const {
            // calc the half-period of the generator
            Generator half_period_generator_one = *this;
            for (ll i = 0; i < period; ++i) {
                half_period_generator_one.generate();
            }
            // now we are going with two generators
            // (from the first generated number and from the n-th)
            // and seeing when it occurs a collision
            Generator half_period_generator_two = *this;
            ll half_period = 0;
            while (half_period_generator_one != half_period_generator_two) {
                half_period_generator_one.generate();
                half_period_generator_two.generate();
                ++half_period;
            }
            return half_period;
        }

        ld CalcZ(ll d,
            const int kgen_numbers,
            const int kinterval_size) const {
            // calc an average deviation among a given number
            // of generated numbers
            ld generated_numbers[kgen_numbers];
            Generator z_helper_generator = *this;
            generated_numbers[0] = static_cast<ld>(z_helper_generator.seed.s1) / d;
            generated_numbers[1] = static_cast<ld>(z_helper_generator.seed.s2) / d;
            generated_numbers[2] = static_cast<ld>(z_helper_generator.seed.s3) / d;
            // genering kgen_numbers - 3 (first three are seed) numbers
            // and normalizing them
            for (int i = 3; i < kgen_numbers; ++i) {
                generated_numbers[i] = static_cast<ld>(
                    z_helper_generator.generate()) / d;
            }
            int distribution[kinterval_size] = {0};
            for (int i = 0; i < kgen_numbers; ++i) {
                // spliting intervals
                ++distribution[(
                    static_cast<int>(generated_numbers[i] * kinterval_size))];
            }
            ld z = 0;
            for (int value : distribution) {
                int numerator = (value - kinterval_size) * (value - kinterval_size);
                int denominator = kinterval_size * kinterval_size;
                z += static_cast<ld>(numerator) / denominator;
            }
            return z;
        }

    private:
        Coefficients cfs_;
        ll mod(ll x, ll m) const {
            ll result = x % m;
            if (result < 0) {
                result += m;
            }
            return result;
        }
};


int main() {
    ll a, b, c, d, x1, x2, x3;
    std::cin >> a >> b >> c >> d >> x1 >> x2 >> x3;
    const Generator::Seed seed(x1, x2, x3);
    const Generator::Coefficients cfs(a, b, c, d);
    const Generator generator(cfs, seed);

    ll period = generator.CalcPeriod();
    ll half_period = generator.CalcHalfPeriod(period);
    ld z = generator.CalcZ(d, 400, 20);

    std::cout << std::fixed << std::setprecision(3);
    std::cout << half_period << " " << period << " " << z;
    return 0;
}
