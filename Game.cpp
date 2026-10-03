#include "Game.hpp"

Game::Game() :
    mTextureManager(),
    mAnimationHandler(mTextureManager),
    mPlayer(mTextureManager, mAnimationHandler),
    mBackGroundTexture(),
    mBackGround(mBackGroundTexture),
    mWindow(sf::VideoMode(mWindowSize), "myGame", sf::Style::Default),
    mFont("assets/fonts/arial.ttf"),
    mFpsText(mFont),
    mView(sf::FloatRect({ 0.f, 0.f }, sf::Vector2f(mWindowSize)))
{   
    mAnimationHandler.load_sprites_textures();
    mPlayer.initialize_sprites_textures();
    mWindow.setFramerateLimit(60);

    loadMapData(jsonpath, map);

    mProbs.matchTextures();

    mFpsText.setCharacterSize(20);
    mFpsText.setFillColor(sf::Color::Blue);
    mFpsText.setPosition({ 5.f, 5.f });
    mFpsText.setString("FPS: 0");

    //listening_socket = server.initialize_server(listening_socket);
    client_socket = client.initialize_client();
}

void Game::run() {
    while (mWindow.isOpen()) {
        processEvent();

        sf::Time dt = mClock.restart();
        globalTime += dt;
        simulationTime += dt;
       
        while (simulationTime >= timePerTick) {
            simulationTime -= timePerTick;
            processEvent();
            update(timePerTick);
        }
        int timetonexttick = std::max(0, timePerTick.asMilliseconds() - simulationTime.asMilliseconds());
        std::cout << "time till lastframe drawed: " << simulationTime.asMilliseconds() << " timepertick: " << timePerTick.asMilliseconds() << std::endl;

        std::uint8_t movementflag = inputhandl.collectInput();
        std::cout << "movement flag: " << std::bitset<8>(movementflag) << std::endl;

        int sent_bytes = client.send_movementflag(client_socket, movementflag);
        movementflag = 0;
        std::cout << "gitti olum paketler laaan sayi: " << sent_bytes << std::endl;

        //crashes at 2.iteration out of range vector.
        // solved the crash somehow but this function is fucking slow affff
        // and there happens a infinite loop between update and fixed time step due being slow ig.
        
        std::vector<uint8_t> pstate_buffer = client.read_playerstate_from_server(client_socket, bytes, timetonexttick);

        if(bytes > 0){
            std::cout << "serverdan okunan bytelar 0 dan buyuk: " << bytes << std::endl;  
            p_state = client.deserialize_playerstate(pstate_buffer);
        }
        else
            std::cout << "gitmedi paketler " << std::endl;    

         // segment fault line.

        fpsTimer += dt;

        if (fpsTimer.asSeconds() > 1.5f) {
            int fps = 1.0f / dt.asSeconds();
            mFpsText.setString("FPS: " + std::to_string(fps));
            fpsTimer = sf::Time::Zero;
        }

        render();
    }
}

void Game::processEvent() {
    while (const std::optional event = mWindow.pollEvent()) {
        if (event->is < sf::Event::Closed>()) {
            mWindow.close();
        }

        if (const auto* resized = event->getIf<sf::Event::Resized>()) {
            float nsx = (float)resized->size.x;
            float nsy = (float)resized->size.y;
            mView.setSize({ nsx, nsy });
            mView.setCenter({ nsx / 2, nsy / 2 });
        }
    }
}

void Game::update(sf::Time dt) {

    mPlayer.update(dt, mWindow.hasFocus(), p_state);
    
    mCollisionManager.screen_collision(mPlayer);

    map.update(dt);
}

void Game::render() {
    mWindow.clear();

    mView.setCenter({std::round(mPlayer.getPosition().x), std::round(mPlayer.getPosition().y)});
    mWindow.setView(mView);

    mWindow.draw(map);

    mProbs.DrawProbs(mWindow);
    mPlayer.draw(mWindow);
    mCollisionManager.draw_screen_collision_borders(mWindow);
        
    mWindow.setView(mWindow.getDefaultView());
    mWindow.draw(mFpsText);

    mWindow.display();
}