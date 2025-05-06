FROM  learnwithexamples/systemc:0.0
WORKDIR /workspace
COPY ./  /workspace/
RUN make SYSTEMC_HOME=/usr/local/systemc-2.3.3 SYSTEMC_LIB=/usr/local/systemc-2.3.3/lib-linux64




