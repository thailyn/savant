rem Additional option: --skip-unrated
rem --unrated-rating value must be non-negative ([0, 100] is meaningful)
savant.exe --input=library.xml --playlist=Music --rating-weight=iTunes --unrated-rating=0 --output=playlist.m3u -n 25
