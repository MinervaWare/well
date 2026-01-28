WELL= ../../../well
WFLAGS= -i -cf "-ldiscord -lcurl -lpthread"

all: a.out

a.out:
	$(WELL) main.well $(WFLAGS)

clean:
	rm *.o *.s a.out
