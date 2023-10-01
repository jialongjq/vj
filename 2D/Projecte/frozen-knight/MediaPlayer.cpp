#include "MediaPlayer.h"

MediaPlayer* MediaPlayer::mediaPlayer;

MediaPlayer::MediaPlayer() {
	soundEngine = irrklang::createIrrKlangDevice();
	musicEngine = irrklang::createIrrKlangDevice();
	musicEngine->setSoundVolume(irrklang::ik_f32(0.4));

	arrowSound = soundEngine->addSoundSourceFromFile("media/tick.x-wav");
	openSound = soundEngine->addSoundSourceFromFile("media/open.x-wav");
	exitSound = soundEngine->addSoundSourceFromFile("media/close.x-wav");
	invulnerabilitySound = soundEngine->addSoundSourceFromFile("media/invulnerability.x-wav");

	batSound = soundEngine->addSoundSourceFromFile("media/bat.wav");
	vampireSound = soundEngine->addSoundSourceFromFile("media/vampire.wav");
	transformationSound = soundEngine->addSoundSourceFromFile("media/vampire_transform.wav");
	ghostSound = soundEngine->addSoundSourceFromFile("media/ghost.wav");
	skeletonSound = soundEngine->addSoundSourceFromFile("media/skeleton.wav");
	teleportSound = soundEngine->addSoundSourceFromFile("media/teleport.wav");
	clearedSound = soundEngine->addSoundSourceFromFile("media/cleared.wav");
	pointSound = soundEngine->addSoundSourceFromFile("media/point.wav");

	frozenSound = soundEngine->addSoundSourceFromFile("media/frozen.wav");
	jumpSound = soundEngine->addSoundSourceFromFile("media/jump.wav");
	tileSound = soundEngine->addSoundSourceFromFile("media/tile.wav");
	tickSound = soundEngine->addSoundSourceFromFile("media/tick.wav");

	dropSound = soundEngine->addSoundSourceFromFile("media/drop.wav");
	unlockSound = soundEngine->addSoundSourceFromFile("media/unlock.wav");
	shieldSound = soundEngine->addSoundSourceFromFile("media/shield.wav");

	collectableShownSound = soundEngine->addSoundSourceFromFile("media/collectable_shown.wav");
	collectableCollectedSound = soundEngine->addSoundSourceFromFile("media/collectable_collected.wav");

	gameOverSound = soundEngine->addSoundSourceFromFile("media/fart.wav");
	hallelujahSound = soundEngine->addSoundSourceFromFile("media/hallelujah.wav");

	shakingSound = soundEngine->addSoundSourceFromFile("media/shaking2.wav");

	readySound = soundEngine->addSoundSourceFromFile("media/ready.wav");

	level1Music = musicEngine->addSoundSourceFromFile("media/level1.mp3");
	level2Music = musicEngine->addSoundSourceFromFile("media/level2.mp3");
	level3Music = musicEngine->addSoundSourceFromFile("media/level3.mp3");

	titleMusic = musicEngine->addSoundSourceFromFile("media/title.mp3");

}

void MediaPlayer::playArrowSound() {
	soundEngine->play2D(arrowSound);
}

void MediaPlayer::playOpenSound() {
	soundEngine->play2D(openSound);
}

void MediaPlayer::playExitSound() {
	soundEngine->play2D(exitSound);
}

void MediaPlayer::playInvulnerabilitySound() {
	soundEngine->play2D(invulnerabilitySound);
}

void MediaPlayer::playEnemySound(string type) {
	if (type == "skeleton") {
		soundEngine->play2D(skeletonSound);
	}
	else if (type == "vampire") {
		soundEngine->play2D(vampireSound);
	}
	else if (type == "bat") {
		soundEngine->play2D(batSound);
	}
	else if (type == "transformation") {
		soundEngine->play2D(transformationSound);
	}
	else if (type == "ghost") {
		soundEngine->play2D(ghostSound);
	}
}

void MediaPlayer::playSpawnSound() {
	soundEngine->play2D(teleportSound);
}

void MediaPlayer::playClearedSound() {
	soundEngine->play2D(clearedSound);
}

void MediaPlayer::playPointSound() {
	soundEngine->play2D(pointSound);
}

void MediaPlayer::playJumpSound() {
	soundEngine->play2D(jumpSound);
}

void MediaPlayer::playFrozenSound() {
	soundEngine->play2D(frozenSound);
}
void MediaPlayer::playTileSound() {
	soundEngine->play2D(tileSound);
}
void MediaPlayer::playTickSound() {
	soundEngine->play2D(tickSound);
}

void MediaPlayer::playDropSound() {
	soundEngine->play2D(dropSound);
}

void MediaPlayer::playUnlockSound() {
	soundEngine->play2D(unlockSound);
}

void MediaPlayer::playShieldSound() {
	soundEngine->play2D(shieldSound);
}

void MediaPlayer::playCollectableShownSound() {
	soundEngine->play2D(collectableShownSound);
}

void MediaPlayer::playCollectableCollectedSound() {
	soundEngine->play2D(collectableCollectedSound);
}

void MediaPlayer::playGameOverSound() {
	soundEngine->play2D(gameOverSound);
}

void MediaPlayer::playHallelujahSound() {
	soundEngine->play2D(hallelujahSound);
}

void MediaPlayer::playShakingSound() {
	soundEngine->play2D(shakingSound);
}

void MediaPlayer::playReadySound() {
	soundEngine->play2D(readySound);
}

void MediaPlayer::playLevelMusic(int level) {
	if (level == 1) {
		musicEngine->play2D(level1Music, true);
	}
	else if (level == 2) {
		musicEngine->play2D(level2Music, true);
	}
	else if (level == 3) {
		musicEngine->play2D(level3Music, true);
	}
}

void MediaPlayer::stopAll() {
	musicEngine->stopAllSounds();
	soundEngine->stopAllSounds();
}

void MediaPlayer::stopLevelMusic(int level) {
	if (level == 1) {
		musicEngine->stopAllSoundsOfSoundSource(level1Music);
	}
	else if (level == 2) {
		musicEngine->stopAllSoundsOfSoundSource(level2Music);
	}
	else if (level == 3) {
		musicEngine->stopAllSoundsOfSoundSource(level3Music);
	}
}

void MediaPlayer::playTitleMusic() {
	musicEngine->play2D(titleMusic, true);
}

MediaPlayer* MediaPlayer::getInstance() {
	if (mediaPlayer == nullptr) {
		mediaPlayer = new MediaPlayer();
	}
	return mediaPlayer;
}