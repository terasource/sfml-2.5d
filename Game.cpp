#include "Game.hpp"

Game::Game() :
    mTextureManager(),
    mAnimationHandler(mTextureManager),
    mBackGroundTexture(),
    mBackGround(mBackGroundTexture),
    mWindow(sf::VideoMode(mWindowSize), "myGame", sf::Style::Default),
    mFont("assets/fonts/arial.ttf"),
    mFpsText(mFont),
    mView(sf::FloatRect({ 0.f, 0.f }, sf::Vector2f(mWindowSize)))
{
    client_socket = client.initialize_client();
    auto frames = client.recv_all(client_socket, bytes, 200);
    auto msgs = client.crbs_to_msg(frames);

    client_player_id = client.get_player_id(msgs);
    mAnimationHandlerList.insert({ client_player_id, mAnimationHandler });
    player_list.insert({ client_player_id, Player(client_player_id,
        playerState{ client_player_id,0,0 },
        mTextureManager,
        mAnimationHandlerList.at(client_player_id)
        ) });

    mainPlayer = &player_list.at(client_player_id);

    std::cerr << "client player id = " << static_cast<int>(client_player_id) << std::endl;

    //initializing.
    for (size_t i = 0; i < msgs.size(); i++) {

        auto it = player_list.find(msgs[i].readpid());

        if (msgs[i].type() == messageType::playerState && it == player_list.end()) {
            auto ps = msgs[i].readps();
            std::cout << "111" << std::endl;

            auto animhandl = AnimationHandler(mTextureManager);
            mAnimationHandlerList.insert({ ps.id, animhandl });

            auto np = Player(ps.id, ps, mTextureManager, mAnimationHandlerList.at(ps.id));

            player_list.insert({ ps.id,np });

        }
    }

    for (auto& animhandl : mAnimationHandlerList) {
        animhandl.second.load_sprites_textures();
    }

    for (auto& player : player_list) {
        player.second.initialize_sprites_textures();
    }

    mWindow.setFramerateLimit(60);

    loadMapData(jsonpath, map);

    mProbs.matchTextures();

    mFpsText.setCharacterSize(20);
    mFpsText.setFillColor(sf::Color::Blue);
    mFpsText.setPosition({ 5.f, 5.f });
    mFpsText.setString("FPS: 0");

}

void Game::run() {
    while (mWindow.isOpen()) {
        processEvent();

        sf::Time dt = mClock.restart();
        globalTime += dt;
        simulationTime += dt;

        int timetonexttick = std::max(0, timePerTick.asMilliseconds() - simulationTime.asMilliseconds());
        //std::cout << "time till lastframe drawed: " << simulationTime.asMilliseconds() << " timepertick: " << timePerTick.asMilliseconds() << std::endl;

        if (mWindow.hasFocus()) {
            std::uint8_t movementflag = inputhandl.collectInput();

            //std::cout << "movement flag: " << std::bitset<8>(movementflag) << std::endl;

            if (movementflag > 0) {
                int sent_bytes = client.send_movementflag(client_socket, movementflag);
                //std::cout << "gitti olum paketler laaan sayi: " << sent_bytes << std::endl;
            }
        }

        auto frames = client.recv_all(client_socket, bytes, timetonexttick);
        auto msgs = client.crbs_to_msg(frames);

        for (size_t i = 0; i < msgs.size(); i++) {
            if (msgs[i].type() == messageType::playerID) {
                auto pid = msgs[i].readpid();

                auto animhandl = AnimationHandler(mTextureManager);
                playerState ps{ pid, 0, 0 };
                mAnimationHandlerList.insert({ pid, animhandl });

                Player new_player = Player(pid, ps, mTextureManager, mAnimationHandlerList.at(pid));

                player_list.insert({ pid,new_player });

                mAnimationHandlerList.at(pid).load_sprites_textures();
                player_list.at(pid).initialize_sprites_textures();

            }
        }

        player_state_list = client.deserialize_playerstates(msgs, player_state_list);

        for (auto player_state : player_state_list) {
            auto it = player_list.find(player_state.id);

            if (it != player_list.end())
                it->second.set_player_state(player_state);
        }

        fpsTimer += dt;

        while (simulationTime >= timePerTick) {
            simulationTime -= timePerTick;
            processEvent();
            update(timePerTick);
        }


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


    for (auto& [id, player] : player_list) {
        player.update(dt, mWindow.hasFocus());
    }
    //mCollisionManager.screen_collision(mPlayer);

    map.update(dt);
}

void Game::render() {
    mWindow.clear();

    if (player_list.size() > 0) {
        mView.setCenter({ std::round(mainPlayer->getPosition().x), std::round(mainPlayer->getPosition().y) });
    }

    mWindow.setView(mView);

    mWindow.draw(map);

    mProbs.DrawProbs(mWindow);

    for (auto& [id, player] : player_list) {
        player.draw(mWindow);
        //std::cout << "player list size = " << player_list.size() << "\n";
        //std::cout << "player # " << i << " is being drawed\n";
    }

    mCollisionManager.draw_screen_collision_borders(mWindow);

    mWindow.setView(mWindow.getDefaultView());
    mWindow.draw(mFpsText);

    mWindow.display();
}