#include <precompiledengine.h>
#include <engine/iengine.h>
#include <asteroids/asteroids.h>

int main()
{

    auto asteroids = std::make_unique<ast::Asteroids>();
    IEngine::run( std::move( asteroids ) );

    return 0;
}