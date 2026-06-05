#pragma once

namespace PistonSlap{
    class Graphics {
        public:
            Graphics() = default;
            ~Graphics();
            Graphics(const Graphics &) = delete;
            Graphics(Graphics &&) = delete;
            Graphics &operator=(const Graphics &) = delete;
            Graphics &operator=(Graphics &&) = delete;

        private:
    };
};