/**
 * SLeeLa &rarr; Java connector: a transport-neutral integration contract that
 * lets a Java host invoke SLeeLa operations without depending on how SLeeLa is
 * reached.
 *
 * <h2>Contract</h2>
 * <ul>
 *   <li>{@link com.mearvk.sleela.connector.SleelaJavaConnector} &mdash; the
 *       interface a Java application programs against ({@code invoke},
 *       {@code health}, {@code isHealthy}, {@code close}). It is
 *       {@link java.lang.AutoCloseable}, so use it with try-with-resources.</li>
 *   <li>{@link com.mearvk.sleela.connector.SleelaInvocation} &mdash; an
 *       immutable, validated request record ({@code operation} +
 *       {@code arguments}); build one with the {@code of(...)} factories.</li>
 *   <li>{@link com.mearvk.sleela.connector.SleelaResult} &mdash; an immutable,
 *       validated result record ({@code success}/{@code value}/{@code error});
 *       build one with {@code SleelaResult.success(...)} /
 *       {@code SleelaResult.failure(...)}.</li>
 * </ul>
 *
 * <h2>Transports (interchangeable implementations)</h2>
 * <ul>
 *   <li><b>process</b> &mdash;
 *       {@link com.mearvk.sleela.connector.process.SleelaProcessConnector}
 *       launches the native SLeeLa executable locally. Its {@code health()} is a
 *       real filesystem liveness check (executable present + runnable, working
 *       directory present).</li>
 *   <li><b>rmi</b> &mdash;
 *       {@link com.mearvk.sleela.connector.rmi.SleelaRmiConnector} adapts the
 *       Java RMI SLeeLa service.</li>
 *   <li><b>http</b> &mdash;
 *       {@link com.mearvk.sleela.connector.http.SleelaHttpConnector} is a
 *       {@link java.net.http.HttpClient} client (with a request timeout) for a
 *       SLeeLa HTTP gateway.</li>
 * </ul>
 *
 * <h2>Error model</h2>
 * {@code invoke(...)} returns a {@link com.mearvk.sleela.connector.SleelaResult}
 * rather than throwing: any transport or execution fault is folded into a
 * {@code failure(...)} result, so callers branch on
 * {@link com.mearvk.sleela.connector.SleelaResult#success()} instead of catching
 * exceptions. {@code health()} may throw, and {@code isHealthy()} wraps it into
 * a boolean.
 *
 * <h2>Selecting a transport</h2>
 * Because every transport implements the same
 * {@link com.mearvk.sleela.connector.SleelaJavaConnector}, a host can choose or
 * swap transports without changing call sites:
 * <pre>{@code
 * try (SleelaJavaConnector c = new SleelaProcessConnector(exe, cwd)) {
 *     if (c.isHealthy()) {
 *         SleelaResult r = c.invoke(SleelaInvocation.of("greet", "world"));
 *         if (r.success()) System.out.println(r.value());
 *         else             System.err.println(r.error());
 *     }
 * }
 * }</pre>
 */
package com.mearvk.sleela.connector;
