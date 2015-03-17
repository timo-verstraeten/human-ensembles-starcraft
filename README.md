## Config file

- Should be saved as `config.ini` in `F:\Starcraft`.
- Should create folder `C:\Trials` (used as a fallback)

Using following configuration, the log and all trials output will be saved under `G:\Project\Test`. Note that this folder **has to exist**.

```ini
[TRIAL]
alpha = 0.40
lambda = 0.90
numEpisodes = 200
resolutionScale = 1.0
numTilings = 1
shapingPotentials = distance:25 human:25 human:50
;loadInitialWeights = ./weights
humanAdviceEpisodes = 5
humanAdviceGamma = 0.5

[EXPERIMENT]
name = Test
numTrials = 5

[GENERAL]
outputPath = G:\Project\
enableLogging = true
saveWeights = false
```
